/* SPDX-License-Identifier: GPL-2.0-only */
/* gain-time-scale conversion helpers for IIO light sensors
 *
 * Copyright (c) 2023 Matti Vaittinen <mazziesaccount@gmail.com>
 */

#ifndef __GAIN_TIME_SCALE_HELPER__
#define __GAIN_TIME_SCALE_HELPER__

/**
 * struct iio_gain_sel_pair - gain - selector values
 *
 * In many cases devices like light sensors allow setting signal amplification
 * (gain) using a register interface. This structure describes amplification
 * and corresponding selector (register value)
 *
 * @gain:	Gain (multiplication) value.
 * @sel:	Selector (usually register value) used to indicate this gain
 */
struct iio_gain_sel_pair {
	int gain;
	int sel;
};

/**
 * struct iio_itime_sel_mul - integration time description
 *
 * In many cases devices like light sensors allow setting the duration of
 * collecting data. Typically this duration has also an impact to the magnitude
 * of measured values (gain). This structure describes the relation of
 * integration time and amplification as well as corresponding selector
 * (register value).
 *
 * An example could be a sensor allowing 50, 100, 200 and 400 mS times. The
 * respective multiplication values could be 50 mS => 1, 100 mS => 2,
 * 200 mS => 4 and 400 mS => 8 assuming the impact of integration time would be
 * linear in a way that when collecting data for 50 mS caused value X, doubling
 * the data collection time caused value 2X etc..
 *
 * @time_us:	Integration time in microseconds.
 * @sel:	Selector (usually register value) used to indicate this time
 * @mul:	Multiplication to the values caused by this time.
 */
struct iio_itime_sel_mul {
	int time_us;
	int sel;
	int mul;
};

struct iio_gts {
	u64 max_scale;
	const struct iio_gain_sel_pair *hwgain_table;
	int num_hwgain;
	const struct iio_itime_sel_mul *itime_table;
	int num_itime;
};

#define GAIN_SCALE_GAIN(_gain, _sel)			\
{							\
	.gain = (_gain),				\
	.sel = (_sel),					\
}

#define GAIN_SCALE_ITIME_MS(_itime, _sel, _mul)		\
{							\
	.time_us = (_itime) * 1000,				\
	.sel = (_sel),					\
	.mul = (_mul),					\
}

#define GAIN_SCALE_ITIME_US(_itime, _sel, _mul)		\
{							\
	.time_us = (_itime),				\
	.sel = (_sel),					\
	.mul = (_mul),					\
}

int iio_init_iio_gts(int max_scale_int, int max_scale_nano,
		     const struct iio_gain_sel_pair *gain_tbl, int num_gain,
		     const struct iio_itime_sel_mul *tim_tbl, int num_times,
		     struct iio_gts *gts);

bool iio_gts_valid_gain(struct iio_gts *gts, int gain);
bool iio_gts_valid_time(struct iio_gts *gts, int time_us);

int iio_gts_get_total_gain_by_sel(struct iio_gts *gts, int gsel, int tsel);
int iio_gts_get_total_gain(struct iio_gts *gts, int gain, int time);

int iio_gts_find_gain_by_sel(struct iio_gts *gts, int sel);
int iio_gts_find_sel_by_gain(struct iio_gts *gts, int gain);
int iio_gts_find_int_time_by_sel(struct iio_gts *gts, int sel);
int iio_gts_find_sel_by_int_time(struct iio_gts *gts, int time);

int iio_gts_get_int_time_gain_multiplier_by_sel(struct iio_gts *gts,
						       int sel);
int iio_gts_find_gain_sel_for_scale_using_time(struct iio_gts *gts, int time_sel,
					       int scale_int, int scale_nano,
					       int *gain_sel);
int iio_gts_find_gain_for_scale_using_time(struct iio_gts *gts, int time_sel,
					   int scale_int, int scale_nano,
					   int *gain);
int iio_gts_find_time_and_gain_sel_for_scale(struct iio_gts *gts, int scale_int,
					     int scale_nano, int *gain_sel,
					     int *time_sel);
int iio_gts_get_scale(struct iio_gts *gts, int gain, int time, int *scale_int,
		      int *scale_nano);
int iio_gts_find_new_gain_sel_by_old_gain_time(struct iio_gts *gts,
					       int old_gain, int old_time_sel,
					       int new_time_sel, int *new_gain);

#endif
