// SPDX-License-Identifier: GPL-2.0-only
/* gain-time-scale conversion helpers for IIO light sensors
 *
 * Copyright (c) 2023 Matti Vaittinen <mazziesaccount@gmail.com>
 */

#include <linux/errno.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/units.h>

#include "gain-time-scale-helper.h"

static int iio_gts_get_gain(const u64 max, u64 scale)
{
	int tmp = 1;

	if (scale > max || !scale)
		return -EINVAL;

	if (0xffffffffffffffffLLU - max < scale) {
		/* Risk of overflow */
		if (max - scale < scale)
			return 1;

		while (max - scale > scale * (u64) tmp)
			tmp++;

		return tmp + 1;
	}

	while (max > scale * (u64) tmp)
		tmp++;

	return tmp;
}

static int gain_get_scale_fraction(const u64 max, u64 scale, int known,
				   int *unknown)
{
	int tot_gain;

	if (!known)
		return -EINVAL;

	tot_gain = iio_gts_get_gain(max, scale);
	if (tot_gain < 0)
		return tot_gain;

	*unknown = tot_gain/known;

	/* We require total gain to be exact multiple of known * unknown */
	if (!*unknown || *unknown * known != tot_gain)
		return -EINVAL;

	return 0;
}

static const struct iio_itime_sel_mul *
			iio_gts_find_itime_by_time(struct iio_gts *gts, int time)
{
	int i;

	if (!gts->num_itime)
		return NULL;

	for (i = 0; i < gts->num_itime; i++)
		if (gts->itime_table[i].time_us == time)
			return &gts->itime_table[i];

	return NULL;
}

static const struct iio_itime_sel_mul *
			iio_gts_find_itime_by_sel(struct iio_gts *gts, int sel)
{
	int i;

	if (!gts->num_itime)
		return NULL;

	for (i = 0; i < gts->num_itime; i++)
		if (gts->itime_table[i].sel == sel)
			return &gts->itime_table[i];

	return NULL;
}

static int iio_gts_delinearize(u64 lin_scale, int *scale_whole, int *scale_nano,
			       unsigned long scaler)
{
	int frac;

	if (scaler > NANO || !scaler)
		return -EINVAL;

	frac = do_div(lin_scale, scaler);

	*scale_whole = lin_scale;
	*scale_nano = frac * (NANO / scaler);

	return 0;
}

static int iio_gts_linearize(int scale_whole, int scale_nano, u64 *lin_scale,
			     unsigned long scaler)
{
	/*
	 * Expect scale to be (mostly) NANO or MICRO. Divide divider instead of
	 * multiplication followed by division to avoid overflow
	 */
	if (scaler > NANO || !scaler)
		return -EINVAL;

	*lin_scale = (u64) scale_whole * (u64)scaler + (u64)(scale_nano
		     / (NANO / scaler));

	return 0;
}

/**
 * iio_init_iio_gts - Initialize the gain-time-scale helper
 * @max_scale_int:	integer part of the maximum scale value
 * @max_scale_nano:	fraction part of the maximum scale value
 * @gain_tbl:		table describing supported gains
 * @num_gain:		number of gains in the gaintable
 * @tim_tbl:		table describing supported integration times
 * @num_times:		number of times in the time table
 * @gts:		pointer to the helper struct
 *
 * Initialize the gain-time-scale helper for use. Please, provide the
 * integration time table sorted so that the preferred integration time is
 * in the first array index. The search functions like the
 * iio_gts_find_time_and_gain_sel_for_scale() start search from first
 * provided time.
 *
 * Return: 0 on success.
 */
int iio_init_iio_gts(int max_scale_int, int max_scale_nano,
		     const struct iio_gain_sel_pair *gain_tbl, int num_gain,
		     const struct iio_itime_sel_mul *tim_tbl, int num_times,
		     struct iio_gts *gts)
{
	int ret;

	ret = iio_gts_linearize(max_scale_int, max_scale_nano,
				   &gts->max_scale, NANO);
	if (ret)
		return ret;

	gts->hwgain_table = gain_tbl;
	gts->num_hwgain = num_gain;
	gts->itime_table = tim_tbl;
	gts->num_itime = num_times;

	return 0;
}
EXPORT_SYMBOL_GPL(iio_init_iio_gts);

bool iio_gts_valid_time(struct iio_gts *gts, int time_us)
{
	return !!iio_gts_find_itime_by_time(gts, time_us);
}
EXPORT_SYMBOL_GPL(iio_gts_valid_time);

int iio_gts_find_sel_by_gain(struct iio_gts *gts, int gain)
{
	int i;

	for (i = 0; i < gts->num_hwgain; i++)
		if (gts->hwgain_table[i].gain == gain)
			return gts->hwgain_table[i].sel;

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(iio_gts_find_sel_by_gain);

bool iio_gts_valid_gain(struct iio_gts *gts, int gain)
{
	return !(iio_gts_find_sel_by_gain(gts, gain) < 0);
}
EXPORT_SYMBOL_GPL(iio_gts_valid_gain);

int iio_gts_find_gain_by_sel(struct iio_gts *gts, int sel)
{
	int i;

	for (i = 0; i < gts->num_hwgain; i++)
		if (gts->hwgain_table[i].sel == sel)
			return gts->hwgain_table[i].gain;

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(iio_gts_find_gain_by_sel);

int iio_gts_get_int_time_gain_multiplier_by_sel(struct iio_gts *gts,
						       int sel)
{
	const struct iio_itime_sel_mul *time;

	time = iio_gts_find_itime_by_sel(gts, sel);
	if (!time)
		return -EINVAL;

	return time->mul;
}
EXPORT_SYMBOL_GPL(iio_gts_get_int_time_gain_multiplier_by_sel);

/**
 * iio_gts_find_gain_for_scale_using_time - Find gain by time and scale
 * @gts:	Gain time scale descriptor
 * @time_sel:	Integration time selector correspondig to the time gain is
 *		searhed for
 * @scale_int:	Integral part of the scale (typically val1)
 * @scale_nano:	Fractional part of the scale (nano or ppb)
 * @gain:	Pointer to value where gain is stored.
 *
 * In some cases the light sensors may want to find a gain setting which
 * corresponds given scale and integration time. Sensors which fill the
 * gain and time tables may use this helper to retrieve the gain.
 *
 * Return:	0 on success. -EINVAL if gain matching the parameters is not
 *		found.
 */
int iio_gts_find_gain_for_scale_using_time(struct iio_gts *gts, int time_sel,
					   int scale_int, int scale_nano,
					   int *gain)
{
	u64 scale_linear;
	int ret, mul;

	ret = iio_gts_linearize(scale_int, scale_nano, &scale_linear, NANO);
	if (ret)
		return ret;

	ret = iio_gts_get_int_time_gain_multiplier_by_sel(gts, time_sel);
	if (ret < 0)
		return ret;

	mul = ret;

	ret = gain_get_scale_fraction(gts->max_scale, scale_linear, mul, gain);

	if (ret || !iio_gts_valid_gain(gts, *gain))
		return -EINVAL;

	return 0;
}
EXPORT_SYMBOL_GPL(iio_gts_find_gain_for_scale_using_time);

/*
 * iio_gts_find_gain_sel_for_scale_using_time - Fetch gain selector.
 * See iio_gts_find_gain_for_scale_using_time() for more information
 */
int iio_gts_find_gain_sel_for_scale_using_time(struct iio_gts *gts, int time_sel,
					       int scale_int, int scale_nano,
					       int *gain_sel)
{
	int gain, ret;

	ret = iio_gts_find_gain_for_scale_using_time(gts, time_sel, scale_int,
						     scale_nano, &gain);
	if (ret)
		return ret;

	ret = iio_gts_find_sel_by_gain(gts, gain);
	if (ret < 0)
		return ret;

	*gain_sel = ret;

	return 0;
}
EXPORT_SYMBOL_GPL(iio_gts_find_gain_sel_for_scale_using_time);

int iio_gts_find_time_and_gain_sel_for_scale(struct iio_gts *gts, int scale_int,
					     int scale_nano, int *gain_sel,
					     int *time_sel)
{
	int ret, i;

	for (i = 0; i < gts->num_itime; i++) {
		*time_sel = gts->itime_table[i].sel;
		ret = iio_gts_find_gain_sel_for_scale_using_time(gts, *time_sel,
					scale_int, scale_nano, gain_sel);
		if (!ret)
			return 0;
	}

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(iio_gts_find_time_and_gain_sel_for_scale);

int iio_gts_find_int_time_by_sel(struct iio_gts *gts, int sel)
{
	const struct iio_itime_sel_mul *itime;

	itime = iio_gts_find_itime_by_sel(gts, sel);
	if (!itime)
		return -EINVAL;

	return itime->time_us;
}
EXPORT_SYMBOL_GPL(iio_gts_find_int_time_by_sel);

int iio_gts_find_sel_by_int_time(struct iio_gts *gts, int time)
{
	const struct iio_itime_sel_mul *itime;

	itime = iio_gts_find_itime_by_time(gts, time);
	if (!itime)
		return -EINVAL;

	return itime->sel;
}
EXPORT_SYMBOL_GPL(iio_gts_find_sel_by_int_time);

int iio_gts_get_total_gain_by_sel(struct iio_gts *gts, int gsel, int tsel)
{
	int ret, tmp;

	ret = iio_gts_find_gain_by_sel(gts, gsel);
	if (ret < 0)
		return ret;

	tmp = ret;

	/*
	 * TODO: Would these helpers provde any value for cases where we just
	 * use table of gains and no integration time? This would be a standard
	 * format for gain table representation and regval => gain / gain =>
	 * regval conversions. OTOH, a dummy table based conversion is a memory
	 * hog in cases where the gain could be computed simply based on simple
	 * multiplication / bit-shift or by linear_ranges helpers.
	 *
	 * Currently we return an error if int-time table is not populated.
	 */
	ret = iio_gts_get_int_time_gain_multiplier_by_sel(gts, tsel);
	if (ret < 0)
		return ret;

	return tmp * ret;
}
EXPORT_SYMBOL_GPL(iio_gts_get_total_gain_by_sel);

int iio_gts_get_total_gain(struct iio_gts *gts, int gain, int time)
{
	const struct iio_itime_sel_mul *itime;

	if (!iio_gts_valid_gain(gts, gain))
		return -EINVAL;

	if (!gts->num_itime)
		return gain;

	itime = iio_gts_find_itime_by_time(gts, time);
	if (!itime)
		return -EINVAL;

	return gain * itime->mul;
}
EXPORT_SYMBOL(iio_gts_get_total_gain);

static int iio_gts_get_scale_linear(struct iio_gts *gts, int gain, int time,
				    u64 *scale)
{
	int total_gain;
	u64 tmp;

	total_gain = iio_gts_get_total_gain(gts, gain, time);
	if (total_gain < 0)
		return total_gain;

	tmp = gts->max_scale;

	do_div(tmp, total_gain);

	*scale = tmp;

	return 0;
}

int iio_gts_get_scale(struct iio_gts *gts, int gain, int time, int *scale_int,
		      int *scale_nano)
{
	u64 lin_scale;
	int ret;

	ret = iio_gts_get_scale_linear(gts, gain, time, &lin_scale);
	if (ret)
		return ret;

	return iio_gts_delinearize(lin_scale, scale_int, scale_nano, NANO);
}
EXPORT_SYMBOL_GPL(iio_gts_get_scale);

/**
 * iio_gts_find_new_gain_sel_by_old_gain_time - compensate time change
 * @gts:		Gain time scale descriptor
 * @old_gain:		Previously set gain
 * @old_time_sel:	Selector corresponding previously set time
 * @new_time_sel:	Selector corresponding new time to be set
 * @new_gain:		Pointer to value where new gain is to be written
 *
 * We may want to mitigate the scale change caused by setting a new integration
 * time (for a light sensor) by also updating the (HW)gain. This helper computes
 * new gain value to maintain the scale with new integration time.
 *
 * Return: 0 on success. -EINVAL if gain matching the new time is not found.
 */
int iio_gts_find_new_gain_sel_by_old_gain_time(struct iio_gts *gts,
					       int old_gain, int old_time_sel,
					       int new_time_sel, int *new_gain)
{
	const struct iio_itime_sel_mul *itime_old, *itime_new;
	u64 scale;
	int ret;

	itime_old = iio_gts_find_itime_by_sel(gts, old_time_sel);
	if (!itime_old)
		return -EINVAL;

	itime_new = iio_gts_find_itime_by_sel(gts, new_time_sel);
	if (!itime_new)
		return -EINVAL;

	ret = iio_gts_get_scale_linear(gts, old_gain, itime_old->time_us,
				       &scale);
	if (ret)
		return ret;

	ret = gain_get_scale_fraction(gts->max_scale, scale, itime_new->mul,
				      new_gain);
	if (ret)
		return -EINVAL;

	if (!iio_gts_valid_gain(gts, *new_gain))
		return -EINVAL;

	return 0;
}
EXPORT_SYMBOL_GPL(iio_gts_find_new_gain_sel_by_old_gain_time);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matti Vaittinen <mazziesaccount@gmail.com>");
MODULE_DESCRIPTION("IIO light sensor gain-time-scale helpers");
