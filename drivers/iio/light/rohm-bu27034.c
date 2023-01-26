// SPDX-License-Identifier: GPL-2.0-only
/*
 * BU27034 ROHM Ambient Light Sensor
 *
 * Copyright (c) 2023, ROHM Semiconductor.
 * https://fscdn.rohm.com/en/products/databook/datasheet/ic/sensor/light/bu27034nuc-e.pdf
 */

#include <linux/bits.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/property.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/units.h>

#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>

#include "gain-time-scale-helper.h"

#define BU27034_REG_SYSTEM_CONTROL	0x40
#define BU27034_MASK_SW_RESET		BIT(7)
#define BU27034_MASK_PART_ID		GENMASK(5, 0)
#define BU27034_ID			0x19
#define BU27034_REG_MODE_CONTROL1	0x41
#define BU27034_MASK_MEAS_MODE		GENMASK(2, 0)

#define BU27034_REG_MODE_CONTROL2	0x42
#define BU27034_MASK_D01_GAIN		GENMASK(7, 3)
#define BU27034_SHIFT_D01_GAIN		3
#define BU27034_MASK_D2_GAIN_HI		GENMASK(7, 6)
#define BU27034_MASK_D2_GAIN_LO		GENMASK(2, 0)
#define BU27034_SHIFT_D2_GAIN		3

#define BU27034_REG_MODE_CONTROL3	0x43
#define BU27034_REG_MODE_CONTROL4	0x44
#define BU27034_MASK_MEAS_EN		BIT(0)
#define BU27034_MASK_VALID		BIT(7)
#define BU27034_REG_DATA0_LO		0x50
#define BU27034_REG_DATA1_LO		0x52
#define BU27034_REG_DATA2_LO		0x54
#define BU27034_REG_DATA2_HI		0x55
#define BU27034_REG_MANUFACTURER_ID	0x92
#define BU27034_REG_MAX BU27034_REG_MANUFACTURER_ID

enum {
	BU27034_CHAN_ALS,
	BU27034_CHAN_DATA0,
	BU27034_CHAN_DATA1,
	BU27034_CHAN_DATA2,
	BU27034_NUM_CHANS
};

/*
 * Available scales with gain 1x - 4096x, timings 55, 100, 200, 400 mS
 * Time impacts to gain: 1x, 2x, 4x, 8x.
 *
 * => Max total gain is HWGAIN * gain by integration time (8 * 4096) = 32768
 *
 * Using NANO precision for scale we must use scale 64x corresponding gain 1x
 * to avoid precision loss. (32x would result scale 976 562.5(nanos).
 */
#define BU27034_SCALE_1X	64

#define BU27034_GSEL_1X		0x00
#define BU27034_GSEL_4X		0x08
#define BU27034_GSEL_16X	0x0a
#define BU27034_GSEL_32X	0x0b
#define BU27034_GSEL_64X	0x0c
#define BU27034_GSEL_256X	0x18
#define BU27034_GSEL_512X	0x19
#define BU27034_GSEL_1024X	0x1a
#define BU27034_GSEL_2048X	0x1b
#define BU27034_GSEL_4096X	0x1c

/* Available gain settings */
static const struct iio_gain_sel_pair bu27034_gains[] = {
	GAIN_SCALE_GAIN(1, BU27034_GSEL_1X),
	GAIN_SCALE_GAIN(4, BU27034_GSEL_4X),
	GAIN_SCALE_GAIN(16, BU27034_GSEL_16X),
	GAIN_SCALE_GAIN(32, BU27034_GSEL_32X),
	GAIN_SCALE_GAIN(64, BU27034_GSEL_64X),
	GAIN_SCALE_GAIN(256, BU27034_GSEL_256X),
	GAIN_SCALE_GAIN(512, BU27034_GSEL_512X),
	GAIN_SCALE_GAIN(1024, BU27034_GSEL_1024X),
	GAIN_SCALE_GAIN(2048, BU27034_GSEL_2048X),
	GAIN_SCALE_GAIN(4096, BU27034_GSEL_4096X),
};

/*
 * The IC has 5 modes for sampling time. 5 mS mode is exceptional as it limits
 * the data collection to data0-channel only and cuts the supported range to
 * 10 bit. It is not aupported by the driver.
 *
 * "normal" modes are 55, 100, 200 and 400 mS modes - which do have direct
 * multiplying impact to the register values similar to gain.
 *
 * This means that if meas-mode is changed for example from 400 => 200,
 * the scale is doubled. Eg, time impact to total gain is x1, x2, x4, x8.
 */
#define BU27034_MEAS_MODE_100MS		0
#define BU27034_MEAS_MODE_55MS		1
#define BU27034_MEAS_MODE_200MS		2
#define BU27034_MEAS_MODE_400MS		4

static const struct iio_itime_sel_mul bu27034_itimes[] = {
	GAIN_SCALE_ITIME_MS(400, BU27034_MEAS_MODE_400MS, 8),
	GAIN_SCALE_ITIME_MS(200, BU27034_MEAS_MODE_200MS, 4),
	GAIN_SCALE_ITIME_MS(100, BU27034_MEAS_MODE_100MS, 2),
	GAIN_SCALE_ITIME_MS(50, BU27034_MEAS_MODE_55MS, 1),
};

#define BU27034_CHAN_DATA(_name, _ch2)					\
{									\
	.type = IIO_INTENSITY,						\
	.channel = BU27034_CHAN_##_name,				\
	.channel2 = (_ch2),						\
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |			\
			      BIT(IIO_CHAN_INFO_SCALE),			\
	.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_INT_TIME),	\
	.address = BU27034_REG_##_name##_LO,				\
	.scan_index = BU27034_CHAN_##_name,				\
	.scan_type = {							\
		.sign = 'u',						\
		.realbits = 16,						\
		.storagebits = 16,					\
		.endianness = IIO_LE,					\
	},								\
	.indexed = 1							\
}

static const struct iio_chan_spec bu27034_channels[] = {
	{
		.type = IIO_LIGHT,
		.info_mask_separate = BIT(IIO_CHAN_INFO_PROCESSED),
		.channel = BU27034_CHAN_ALS,
	},
	BU27034_CHAN_DATA(DATA0, IIO_MOD_LIGHT_CLEAR),
	BU27034_CHAN_DATA(DATA1, IIO_MOD_LIGHT_CLEAR),
	BU27034_CHAN_DATA(DATA2, IIO_MOD_LIGHT_IR),
};

struct bu27034_data {
	struct regmap *regmap;
	struct device *dev;
	/*
	 * Protect gain and time during scale adjustment and data reading as
	 * well as the channel data 'cached' flag.
	 */
	struct mutex mutex;
	struct iio_gts gts;
	bool cached;
	__le16 raw[3];
};

struct bu27034_result {
	u16 ch0;
	u16 ch1;
	u16 ch2;
};

static const struct regmap_range bu27034_volatile_ranges[] = {
	{
		.range_min = BU27034_REG_MODE_CONTROL4,
		.range_max = BU27034_REG_MODE_CONTROL4,
	}, {
		.range_min = BU27034_REG_DATA0_LO,
		.range_max = BU27034_REG_DATA2_HI,
	},
};

static const struct regmap_access_table bu27034_volatile_regs = {
	.yes_ranges = &bu27034_volatile_ranges[0],
	.n_yes_ranges = ARRAY_SIZE(bu27034_volatile_ranges),
};

static const struct regmap_range bu27034_read_only_ranges[] = {
	{
		.range_min = BU27034_REG_DATA0_LO,
		.range_max = BU27034_REG_DATA2_HI,
	}, {
		.range_min = BU27034_REG_MANUFACTURER_ID,
		.range_max = BU27034_REG_MANUFACTURER_ID,
	}
};

static const struct regmap_access_table bu27034_ro_regs = {
	.no_ranges = &bu27034_read_only_ranges[0],
	.n_no_ranges = ARRAY_SIZE(bu27034_read_only_ranges),
};

static const struct regmap_config bu27034_regmap = {
	.reg_bits	= 8,
	.val_bits	= 8,

	.max_register	= BU27034_REG_MAX,
	.cache_type	= REGCACHE_RBTREE,
	.volatile_table = &bu27034_volatile_regs,
	.wr_table	= &bu27034_read_only_ranges,
};

static int bu27034_validate_int_time(struct bu27034_data *data, int time_us)
{
	/*
	 * The BU27034 has 55 mS integration time which is in the vendor tests
	 * handled as 50 mS in all of the internal computations. We keep same
	 * approach here.
	 */
	if (time_us == 55000)
		return 50000;

	if (iio_gts_valid_time(&data->gts, time_us))
		return time_us;

	return -EINVAL;
}

struct bu27034_gain_check {
	int old_gain;
	int new_gain;
	int chan;
};

static int bu27034_get_gain_sel(struct bu27034_data *data, int chan)
{
	int ret, val;

	switch (chan) {
	case BU27034_CHAN_DATA0:
	case BU27034_CHAN_DATA1:
	{
		int reg[] = {
			[BU27034_CHAN_DATA0] = BU27034_REG_MODE_CONTROL2,
			[BU27034_CHAN_DATA1] = BU27034_REG_MODE_CONTROL3,
		};
		ret = regmap_read(data->regmap, reg[chan], &val);
		if (ret)
			return ret;

		val &= BU27034_MASK_D01_GAIN;
		return val >> BU27034_SHIFT_D01_GAIN;
	}
	case BU27034_CHAN_DATA2:
		ret = regmap_read(data->regmap, BU27034_REG_MODE_CONTROL2, &val);
		if (ret)
			return ret;

		return (val & BU27034_MASK_D2_GAIN_HI) >> BU27034_SHIFT_D2_GAIN
		       | (val & BU27034_MASK_D2_GAIN_LO);
	}

	dev_err(data->dev, "Can't get gain for channel %d\n", chan);

	return -EINVAL;
}

static int bu27034_get_gain(struct bu27034_data *data, int chan, int *gain)
{
	int ret, sel;

	ret = bu27034_get_gain_sel(data, chan);
	if (ret < 0)
		return ret;

	sel = ret;

	ret = iio_gts_find_gain_by_sel(&data->gts, sel);
	if (ret < 0)
		dev_err(data->dev, "chan %u: unknown gain value 0x%x\n", chan,
			sel);

	*gain = ret;

	return 0;
}

static int bu27034_get_int_time(struct bu27034_data *data)
{
	int ret, sel;

	ret = regmap_read(data->regmap, BU27034_REG_MODE_CONTROL1, &sel);
	if (ret)
		return ret;

	return iio_gts_find_int_time_by_sel(&data->gts,
					    sel & BU27034_MASK_MEAS_MODE);
}

static int _bu27034_get_scale(struct bu27034_data *data, int channel, int *val,
			      int *val2)
{
	int gain, ret;

	ret = bu27034_get_gain(data, channel, &gain);
	if (ret)
		return ret;

	ret = bu27034_get_int_time(data);
	if (ret < 0)
		return ret;

	return iio_gts_get_scale(&data->gts, gain, ret, val, val2);
}

static int bu27034_get_scale(struct bu27034_data *data, int channel, int *val,
			      int *val2)
{
	int ret;

	mutex_lock(&data->mutex);
	ret = _bu27034_get_scale(data, channel, val, val2);
	mutex_unlock(&data->mutex);

	return ret;
}

/* Caller should hold the lock to protect data->cached */
static int bu27034_write_gain_sel(struct bu27034_data *data, int chan, int sel)
{
	static const int reg[] = {
		[BU27034_CHAN_DATA0] = BU27034_REG_MODE_CONTROL2,
		[BU27034_CHAN_DATA1] = BU27034_REG_MODE_CONTROL3,
		[BU27034_CHAN_DATA2] = BU27034_REG_MODE_CONTROL2
	};
	int mask;

	if (chan < BU27034_CHAN_DATA0 || chan > BU27034_CHAN_DATA2)
		return -EINVAL;

	if (chan == BU27034_CHAN_DATA0 || chan == BU27034_CHAN_DATA1) {
		sel <<= BU27034_SHIFT_D01_GAIN;
		mask = BU27034_MASK_D01_GAIN;
	} else {
		/*
		 * We don't allow setting high bits for DATA2 gain because
		 * that impacts to DATA0 as well.
		 */
		mask =  BU27034_MASK_D2_GAIN_LO;
	}

	/* We are changing gain so we need to invalidate cached results. */
	data->cached = false;

	return regmap_update_bits(data->regmap, reg[chan], mask, sel);
}

static int _bu27034_set_gain(struct bu27034_data *data, int chan, int gain)
{
	int ret, sel;

	ret = iio_gts_find_sel_by_gain(&data->gts, gain);
	if (ret < 0)
		return ret;

	return bu27034_write_gain_sel(data, chan, sel);
}

/* Caller should hold the lock to protect data->cached */
static int bu27034_set_int_time(struct bu27034_data *data, int time)
{
	int ret;

	ret = iio_gts_find_sel_by_int_time(&data->gts, time);
	if (ret < 0)
		return ret;

	/* We are changing int time so we need to invalidate cached results. */
	data->cached = false;

	return regmap_update_bits(data->regmap, BU27034_REG_MODE_CONTROL1,
				  BU27034_MASK_MEAS_MODE, ret);
}

/*
 * We try to change the time in such way that the scale is maintained for
 * given channels by adjusting gain so that it compensates the time change.
 */
static int bu27034_try_set_int_time(struct bu27034_data *data, int time_us)
{
	int ret, int_time_old, int_time_new, i;
	struct bu27034_gain_check gains[3] = {
		{ .chan = BU27034_CHAN_DATA0, },
		{ .chan = BU27034_CHAN_DATA1, },
		{ .chan = BU27034_CHAN_DATA2 }
	};
	int numg = ARRAY_SIZE(gains);

	mutex_lock(&data->mutex);
	ret = bu27034_get_int_time(data);
	if (ret < 0)
		goto unlock_out;

	int_time_old = ret;

	ret = bu27034_validate_int_time(data, time_us);
	if (ret < 0) {
		dev_err(data->dev, "Unsupported integration time %u\n",
			time_us);

		goto unlock_out;
	}

	int_time_new = ret;

	if (int_time_new == int_time_old) {
		ret = 0;
		goto unlock_out;
	}

	for (i = 0; i < numg; i++) {
		ret = bu27034_get_gain(data, gains[i].chan,
				       &gains[i].old_gain);
		if (ret)
			goto unlock_out;

		gains[i].new_gain = gains[i].old_gain * int_time_old /
				    int_time_new;

		if (!iio_gts_valid_gain(&data->gts, gains[i].new_gain)) {
			int scale1, scale2;

			_bu27034_get_scale(data, gains[i].chan, &scale1, &scale2);
			dev_err(data->dev,
				"chan %u, can't support time %u with scale %u %u\n",
				gains[i].chan, time_us, scale1, scale2);

			ret = -EINVAL;
			goto unlock_out;
		}
	}

	/*
	 * The new integration time can be supported while keeping the scale of
	 * channels intact by tuning the gains.
	 */
	for (i = 0; i < numg; i++) {
		ret = _bu27034_set_gain(data, gains[i].chan, gains[i].new_gain);
		if (ret)
			goto unlock_out;
	}

	ret = bu27034_set_int_time(data, int_time_new);

unlock_out:
	mutex_unlock(&data->mutex);

	return ret;
}

static int bu27034_set_scale(struct bu27034_data *data, int chan,
			    int val, int val2)
{
	int ret, time_sel, gain_sel, i;
	bool found = false;

	mutex_lock(&data->mutex);
	ret = regmap_read(data->regmap, BU27034_REG_MODE_CONTROL1, &time_sel);
	if (ret)
		goto unlock_out;

	ret = iio_gts_find_gain_sel_for_scale_using_time(&data->gts, time_sel,
						val, val2 * 1000, &gain_sel);
	if (ret) {
		/* We need to maintain the scale for all channels */
		int new_time_sel;
		struct bu27034_gain_check gains[2];

		if (chan == BU27034_CHAN_DATA0) {
			gains[0].chan = BU27034_CHAN_DATA1;
			gains[1].chan = BU27034_CHAN_DATA2;
		} else if (chan == BU27034_CHAN_DATA1) {
			gains[0].chan = BU27034_CHAN_DATA0;
			gains[1].chan = BU27034_CHAN_DATA2;
		} else {
			gains[0].chan = BU27034_CHAN_DATA0;
			gains[1].chan = BU27034_CHAN_DATA1;
		}
		for (i = 0; i < 2; i++) {
			ret = bu27034_get_gain(data, gains[i].chan,
					       &gains[i].old_gain);
			if (ret)
				goto unlock_out;
		}

		for (i = 0; i < data->gts.num_itime; i++) {
			new_time_sel = data->gts.itime_table[i].sel;

			if (new_time_sel == time_sel)
				continue;

			ret = iio_gts_find_gain_sel_for_scale_using_time(
				&data->gts, new_time_sel, val, val2 * 1000,
				&gain_sel);
			if (ret)
				continue;

			ret = iio_gts_find_new_gain_sel_by_old_gain_time(
				&data->gts, gains[0].old_gain, time_sel,
				new_time_sel, &gains[0].new_gain);
			if (ret)
				continue;

			ret = iio_gts_find_new_gain_sel_by_old_gain_time(
				&data->gts, gains[1].old_gain, time_sel,
				new_time_sel, &gains[1].new_gain);
			if (!ret) {
				found = true;
				break;
			}
		}
		if (!found) {
			dev_err(data->dev,
				"Can't set scale maintaining other channels\n");
			ret = -EINVAL;

			goto unlock_out;
		}

		for (i = 0; i < 2; i++) {
			ret = _bu27034_set_gain(data, gains[0].chan,
						gains[0].new_gain);
			if (ret)
				goto unlock_out;

			ret = _bu27034_set_gain(data, gains[1].chan,
						gains[1].new_gain);
			if (ret)
				goto unlock_out;
		}

		ret = regmap_update_bits(data->regmap, BU27034_REG_MODE_CONTROL1,
				  BU27034_MASK_MEAS_MODE, new_time_sel);
		if (ret)
			goto unlock_out;
	}

	ret = bu27034_write_gain_sel(data, chan, gain_sel);
unlock_out:
	mutex_unlock(&data->mutex);

	return ret;
}

/*
 * for (D1/D0 < 0.87):
 * lx = 0.004521097 * D1 - 0.002663996 * D0 +
 *	0.00012213 * D1 * D1 / D0
 *
 * =>	115.7400832 * ch1 / gain1 / mt -
 *	68.1982976 * ch0 / gain0 / mt +
 *	0.00012213 * 25600 * (ch1 / gain1 / mt) * 25600 *
 *	(ch1 /gain1 / mt) / (25600 * ch0 / gain0 / mt)
 *
 * A =	0.00012213 * 25600 * (ch1 /gain1 / mt) * 25600 *
 *	(ch1 /gain1 / mt) / (25600 * ch0 / gain0 / mt)
 * =>	0.00012213 * 25600 * (ch1 /gain1 / mt) *
 *	(ch1 /gain1 / mt) / (ch0 / gain0 / mt)
 * =>	0.00012213 * 25600 * (ch1 / gain1) * (ch1 /gain1 / mt) /
 *	(ch0 / gain0)
 * =>	0.00012213 * 25600 * (ch1 / gain1) * (ch1 /gain1 / mt) *
 *	gain0 / ch0
 * =>	3.126528 * ch1 * ch1 * gain0 / gain1 / gain1 / mt /ch0
 *
 * lx = (115.7400832 * ch1 / gain1 - 68.1982976 * ch0 / gain0) /
 *	mt + A
 * =>	(115.7400832 * ch1 / gain1 - 68.1982976 * ch0 / gain0) /
 *	mt + 3.126528 * ch1 * ch1 * gain0 / gain1 / gain1 / mt /
 *	ch0
 *
 * =>	(115.7400832 * ch1 / gain1 - 68.1982976 * ch0 / gain0 +
 *	  3.126528 * ch1 * ch1 * gain0 / gain1 / gain1 / ch0) /
 *	  mt
 *
 * For (0.87 <= D1/D0 < 1.00)
 * lx = (0.001331* D0 + 0.0000354 * D1) * ((D1/D0 – 0.87) * (0.385) + 1)
 * =>	(0.001331 * 256 * 100 * ch0 / gain0 / mt + 0.0000354 * 256 *
 *	100 * ch1 / gain1 / mt) * ((D1/D0 -  0.87) * (0.385) + 1)
 * =>	(34.0736 * ch0 / gain0 / mt + 0.90624 * ch1 / gain1 / mt) *
 *	((D1/D0 -  0.87) * (0.385) + 1)
 * =>	(34.0736 * ch0 / gain0 / mt + 0.90624 * ch1 / gain1 / mt) *
 *	(0.385 * D1/D0 - 0.66505)
 * =>	(34.0736 * ch0 / gain0 / mt + 0.90624 * ch1 / gain1 / mt) *
 *	(0.385 * 256 * 100 * ch1 / gain1 / mt / (256 * 100 * ch0 / gain0 / mt) - 0.66505)
 * =>	(34.0736 * ch0 / gain0 / mt + 0.90624 * ch1 / gain1 / mt) *
 *	(9856 * ch1 / gain1 / mt / (25600 * ch0 / gain0 / mt) + 0.66505)
 * =>	13.118336 * ch1 / (gain1 * mt)
 *	+ 22.66064768 * ch0 / (gain0 * mt)
 *	+ 8931.90144 * ch1 * ch1 * gain0 /
 *	  (25600 * ch0 * gain1 * gain1 * mt)
 *	+ 0.602694912 * ch1 / (gain1 * mt)
 *
 * =>	[0.3489024 * ch1 * ch1 * gain0 / (ch0 * gain1 * gain1)
 *	 + 22.66064768 * ch0 / gain0
 *	 + 13.721030912 * ch1 / gain1
 *	] / mt
 *
 * For (D1/D0 >= 1.00)
 *
 * lx	= (0.001331* D0 + 0.0000354 * D1) * ((D1/D0 – 2.0) * (-0.05) + 1)
 *	=> (0.001331* D0 + 0.0000354 * D1) * (-0.05D1/D0 + 1.1)
 *	=> (0.001331 * 256 * 100 * ch0 / gain0 / mt + 0.0000354 * 256 *
 *	   100 * ch1 / gain1 / mt) * (-0.05D1/D0 + 1.1)
 *	=> (34.0736 * ch0 / gain0 / mt + 0.90624 * ch1 / gain1 / mt) *
 *	   (-0.05 * 256 * 100 * ch1 / gain1 / mt / (256 * 100 * ch0 / gain0 / mt) + 1.1)
 *	=> (34.0736 * ch0 / gain0 / mt + 0.90624 * ch1 / gain1 / mt) *
 *	   (-1280 * ch1 / (gain1 * mt * 25600 * ch0 / gain0 / mt) + 1.1)
 *	=> (34.0736 * ch0 * -1280 * ch1 * gain0 * mt /( gain0 * mt * gain1 * mt * 25600 * ch0)
 *	    + 34.0736 * 1.1 * ch0 / (gain0 * mt)
 *	    + 0.90624 * ch1 * -1280 * ch1 *gain0 * mt / (gain1 * mt *gain1 * mt * 25600 * ch0)
 *	    + 1.1 * 0.90624 * ch1 / (gain1 * mt)
 *	=> -43614.208 * ch1 / (gain1 * mt * 25600)
 *	    + 37.48096  ch0 / (gain0 * mt)
 *	    - 1159.9872 * ch1 * ch1 * gain0 / (gain1 * gain1 * mt * 25600 * ch0)
 *	    + 0.996864 ch1 / (gain1 * mt)
 *	=> [
 *		- 0.045312 * ch1 * ch1 * gain0 / (gain1 * gain1 * ch0)
 *		- 0.706816 * ch1 / gain1
 *		+ 37.48096  ch0 /gain0
 *	   ] * mt
 *
 *
 * So, the first case (D1/D0 < 0.87) can be computed to a form:
 *
 * lx = (3.126528 * ch1 * ch1 * gain0 / (ch0 * gain1 * gain1) +
 *	 115.7400832 * ch1 / gain1 +
 *	-68.1982976 * ch0 / gain0
 *	 / mt
 *
 * Second case (0.87 <= D1/D0 < 1.00) goes to form:
 *
 *	=> [0.3489024 * ch1 * ch1 * gain0 / (ch0 * gain1 * gain1) +
 *	    13.721030912 * ch1 / gain1 +
 *	    22.66064768 * ch0 / gain0
 *	   ] / mt
 *
 * Third case (D1/D0 >= 1.00) goes to form:
 *	=> [-0.045312 * ch1 * ch1 * gain0 / (ch0 * gain1 * gain1) +
 *	    -0.706816 * ch1 / gain1 +
 *	    37.48096  ch0 /(gain0
 *	   ] / mt
 *
 * This can be unified to format:
 * lx = [
 *	 A * ch1 * ch1 * gain0 / (ch0 * gain1 * gain1) +
 *	 B * ch1 / gain1 +
 *	 C * ch0 / gain0
 *	] / mt
 *
 * For case 1:
 * A = 3.126528,
 * B = 115.7400832
 * C = -68.1982976
 *
 * For case 2:
 * A = 0.3489024
 * B = 13.721030912
 * C = 22.66064768
 *
 * For case 3:
 * A = -0.045312
 * B = -0.706816
 * C = 37.48096
 */

struct bu27034_lx_coeff {
	unsigned int A;
	unsigned int B;
	unsigned int C;
	/* Indicate which of the coefficients above are negative */
	bool is_neg[3];
};

static u64 bu27034_fixp_calc_t1(unsigned int coeff, unsigned int ch0,
				unsigned int ch1, unsigned int gain0,
				unsigned int gain1)
{
	unsigned int helper, tmp;
	u64 helper64;

	/*
	 * Here we could overflow even the 64bit value. Hence we
	 * multiply with gain0 only after the divisions - even though
	 * it may result loss of accuracy
	 */
	helper64 = (u64)coeff * (u64)ch1 * (u64)ch1; /* * (u64)gain0 */
	helper = coeff * ch1 * ch1; /* * gain0*/
	tmp = helper * gain0;

	if (helper == helper64 && (tmp / gain0 == helper))
		return tmp / (gain1 * gain1) / ch0;

	helper = gain1 * gain1;
	if (helper > ch0) {
		do_div(helper64, helper);
		/*
		 * multiplication with max gain may overflow
		 * if helper64 is greater than 0xFFFFFFFFFFFFF.
		 *
		 * If this is the case we divide first.
		 */
		if (helper64 < 0xFFFFFFFFFFFFFLLU) {
			helper64 *= gain0;
			do_div(helper64, ch0);
		} else {
			do_div(helper64, ch0);
			helper64 *= gain0;
		}

		return helper64;
	}

	do_div(helper64, ch0);
	/* Same overflow check here */
	if (helper64 < 0xFFFFFFFFFFFFFLLU) {
		helper64 *= gain0;
		do_div(helper64, helper);
	} else {
		do_div(helper64, helper);
		helper64 *= gain0;
	}

	return helper64;
}

static u64 bu27034_fixp_calc_t23(unsigned int coeff, unsigned int ch,
				 unsigned int gain)
{
	unsigned int helper;
	u64 helper64;

	helper64 = (u64)coeff * (u64)ch;
	helper = coeff * ch;

	if (helper == helper64)
		return helper / gain;

	do_div(helper64, gain);

	return helper64;
}

static int bu27034_fixp_calc_lx(unsigned int ch0, unsigned int ch1,
				unsigned int gain0, unsigned int gain1,
				unsigned int meastime, int coeff_idx)
{
	static const struct bu27034_lx_coeff coeff[] = {
		{
			.A = 31265280,		/* 3.126528 */
			.B = 1157400832,	/*115.7400832 */
			.C = 681982976,		/* -68.1982976 */
			.is_neg = {false, false, true},
		}, {
			.A = 3489024,		/* 0.3489024 */
			.B = 137210309,		/* 13.721030912 */
			.C = 226606476,		/* 22.66064768 */
			/* All terms positive */
		}, {
			.A = 453120,		/* -0.045312 */
			.B = 7068160,		/* -0.706816 */
			.C = 374809600,		/* 37.48096 */
			.is_neg = {true, true, false},
		}
	};
	const struct bu27034_lx_coeff *c = &coeff[coeff_idx];
	u64 res = 0, terms[3];
	int i;

	if (coeff_idx >= ARRAY_SIZE(coeff))
		return -EINVAL;

	terms[0] = bu27034_fixp_calc_t1(c->A, ch0, ch1, gain0, gain1);
	terms[1] = bu27034_fixp_calc_t23(c->B, ch1, gain1);
	terms[2] = bu27034_fixp_calc_t23(c->C, ch0, gain0);

	/* First, add positive terms */
	for (i = 0; i < 3; i++)
		if (!c->is_neg[i])
			res += terms[i];

	/* No positive term => zero lux */
	if (!res)
		return 0;

	/* Then, subtract negative terms (if any) */
	for (i = 0; i < 3; i++)
		if (c->is_neg[i]) {
			/*
			 * If the negative term is greater than positive - then
			 * the darknes has taken over and we are all doomed! Eh,
			 * I mean, then we can just return 0 lx and go out
			 */
			if (terms[i] >= res)
				return 0;

			res -= terms[i];
		}

	meastime *= 10000;
	do_div(res, meastime);

	return (int) res;
}

static bool bu27034_has_valid_sample(struct bu27034_data *data)
{
	int ret, val;

	ret = regmap_read(data->regmap, BU27034_REG_MODE_CONTROL4, &val);
	if (ret)
		dev_err(data->dev, "Read failed %d\n", ret);

	return (val & BU27034_MASK_VALID);
}

static void bu27034_invalidate_read_data(struct bu27034_data *data)
{
	bu27034_has_valid_sample(data);
}

static int _bu27034_get_result(struct bu27034_data *data, u16 *res, bool lock)
{
	int ret = 0;

retry:
	if (lock)
		mutex_lock(&data->mutex);
	/* Get new value from sensor if data is ready - or use cached value */
	if (bu27034_has_valid_sample(data)) {
		ret = regmap_bulk_read(data->regmap, BU27034_REG_DATA0_LO,
				       &data->raw[0], sizeof(data->raw));
		if (ret)
			goto unlock_out;

		data->cached = true;
		bu27034_invalidate_read_data(data);
	} else if (unlikely(!data->cached)) {
		/* No new data in sensor and no value cached. Wait and retry */
		if (lock)
			mutex_unlock(&data->mutex);
		msleep(25);

		goto retry;
	}
	res[0] = le16_to_cpu(data->raw[0]);
	res[1] = le16_to_cpu(data->raw[1]);
	res[2] = le16_to_cpu(data->raw[2]);

unlock_out:
	if (lock)
		mutex_unlock(&data->mutex);

	return ret;
}

static int bu27034_get_result_unlocked(struct bu27034_data *data, u16 *res)
{
	return _bu27034_get_result(data, res, false);
}

static int bu27034_get_result(struct bu27034_data *data, u16 *res)
{
	return _bu27034_get_result(data, res, true);
}

/*
 * The formula given by vendor for computing luxes out of data0 and data1
 * (in open air) is as follows:
 *
 * Let's mark:
 * D0 = data0/ch0_gain/meas_time_ms * 25600
 * D1 = data1/ch1_gain/meas_time_ms * 25600
 *
 * Then:
 * if (D1/D0 < 0.87)
 *	lx = (0.001331 * D0 + 0.0000354 * D1) * ((D1 / D0 - 0.87) * 3.45 + 1)
 * else if (D1/D0 < 1)
 *	lx = (0.001331 * D0 + 0.0000354 * D1) * ((D1 / D0 - 0.87) * 0.385 + 1)
 * else
 *	lx = (0.001331 * D0 + 0.0000354 * D1) * ((D1 / D0 - 2) * -0.05 + 1)
 *
 * we try implementing it here. Users who have for example some colored lens
 * need to modify the calculation but I hope this gives a starting point for
 * those working with such devices.
 *
 * The first case (D1/D0 < 0.87) can be computed to a form:
 * lx = 0.004521097 * D1 - 0.002663996 * D0 + 0.00012213 * D1 * D1 / D0
 */
static int bu27034_get_lux(struct bu27034_data *data, int *val)
{
	unsigned int gain0, gain1, meastime;
	unsigned int d1_d0_ratio_scaled;
	u16 res[3], ch0, ch1;
	u64 helper64;
	int ret;

	mutex_lock(&data->mutex);
	ret = bu27034_get_result_unlocked(data, &res[0]);
	if (ret)
		goto unlock_out;

	/* Avoid div by zero */
	if (!res[0])
		ch0 = 1;
	else
		ch0 = res[0];

	if (!res[1])
		ch1 = 1;
	else
		ch1 = res[1];


	ret = bu27034_get_gain(data, BU27034_CHAN_DATA0, &gain0);
	if (ret)
		goto unlock_out;

	ret = bu27034_get_gain(data, BU27034_CHAN_DATA1, &gain1);
	if (ret)
		goto unlock_out;

	ret = bu27034_get_int_time(data);
	if (ret < 0)
		goto unlock_out;

	meastime = ret;

	mutex_unlock(&data->mutex);

	d1_d0_ratio_scaled = (unsigned int)ch1 * (unsigned int)gain0 * 100;
	helper64 = (u64)ch1 * (u64)gain0 * 100LLU;

	if (helper64 != d1_d0_ratio_scaled) {
		unsigned int div = (unsigned int)ch0 * gain1;

		do_div(helper64, div);
		d1_d0_ratio_scaled = helper64;
	} else {
		d1_d0_ratio_scaled /= ch0 * gain1;
	}

	if (d1_d0_ratio_scaled < 87)
		*val = bu27034_fixp_calc_lx(ch0, ch1, gain0, gain1, meastime, 0);
	else if (d1_d0_ratio_scaled < 100)
		*val = bu27034_fixp_calc_lx(ch0, ch1, gain0, gain1, meastime, 1);
	else
		*val = bu27034_fixp_calc_lx(ch0, ch1, gain0, gain1, meastime, 2);

	return 0;

unlock_out:
	mutex_unlock(&data->mutex);

	return ret;
}

static int bu27034_meas_set(struct bu27034_data *data, bool en)
{
	int ret;

	mutex_lock(&data->mutex);
	if (en)
		ret = regmap_set_bits(data->regmap, BU27034_REG_MODE_CONTROL4,
				      BU27034_MASK_MEAS_EN);
	else
		ret = regmap_clear_bits(data->regmap, BU27034_REG_MODE_CONTROL4,
					BU27034_MASK_MEAS_EN);
	mutex_unlock(&data->mutex);

	return ret;
}

static int bu27034_meas_en(struct bu27034_data *data)
{
	return bu27034_meas_set(data, true);
}

static int bu27034_meas_dis(struct bu27034_data *data)
{
	return bu27034_meas_set(data, false);
}

static int bu27034_read_raw(struct iio_dev *idev,
			   struct iio_chan_spec const *chan,
			   int *val, int *val2, long mask)
{
	struct bu27034_data *data = iio_priv(idev);
	int ret;

	switch (mask) {
	case IIO_CHAN_INFO_INT_TIME:
		*val = 0;

		ret = bu27034_get_int_time(data);
		if (ret < 0)
			return ret;

		/*
		 * We use 50000 uS internally for all calculations and only
		 * convert it to 55000 before returning it to the user.
		 *
		 * This is becaise data-sheet says the time is 55 mS - but
		 * vendor provided computations used 50 mS.
		 */
		if (ret == 50000)
			ret = 55000;

		*val2 = 0;
		*val = ret;

		return IIO_VAL_INT_PLUS_MICRO;

	case IIO_CHAN_INFO_SCALE:
		ret = bu27034_get_scale(data, chan->channel, val, val2);
		if (ret)
			return ret;

		return IIO_VAL_INT_PLUS_NANO;

	case IIO_CHAN_INFO_RAW:
	{
		u16 res[3];

		if (chan->type != IIO_INTENSITY)
			return -EINVAL;

		if (chan->channel < BU27034_CHAN_DATA0 ||
		    chan->channel > BU27034_CHAN_DATA2)
			return -EINVAL;
		/*
		 * Reading one channel at a time is inefficient.
		 *
		 * Hence we run the measurement on the background and always
		 * read all the channels. There are following caveats:
		 * 1) The VALID bit handling is racy. Valid bit clearing is not
		 * tied to reading the data in the hardware. We clear the
		 * valid-bit manually _after_ we have read the data - but this
		 * means there is a small time-window where new result may
		 * arrive between read and clear. This means we can miss a
		 * sample. For normal use this should not be fatal because
		 * usually the light is changing slowly. There might be
		 * use-cases for measuring more rapidly changing light but this
		 * driver is unsuitable for those cases anyways. (Smallest
		 * measurement time we support is 55 mS.)
		 * 2) Data readings more frequent than the meas_time will return
		 * the same cached values. This should not be a problem for the
		 * very same reason 1) is not a problem.
		 */
		ret = bu27034_get_result(data, &res[0]);
		if (ret)
			return ret;

		*val = res[chan->channel - BU27034_CHAN_DATA0];

		return IIO_VAL_INT;
	}

	case IIO_CHAN_INFO_PROCESSED:
		if (chan->type != IIO_LIGHT)
			return -EINVAL;

		ret = bu27034_get_lux(data, val);
		if (ret)
			return ret;
		return IIO_VAL_INT;

	}

	return -EINVAL;
}

static int bu27034_write_raw(struct iio_dev *idev,
			     struct iio_chan_spec const *chan,
			     int val, int val2, long mask)
{
	struct bu27034_data *data = iio_priv(idev);

	switch (mask) {
	case IIO_CHAN_INFO_SCALE:
		return bu27034_set_scale(data, chan->channel, val, val2);
	case IIO_CHAN_INFO_INT_TIME:
		return bu27034_try_set_int_time(data, val);
	}

	return -EINVAL;
}

static const struct iio_info bu27034_info = {
	.read_raw = &bu27034_read_raw,
	.write_raw = &bu27034_write_raw,
};

static void bu27034_meas_stop(void *data)
{
	bu27034_meas_dis(data);
}

static int bu27034_chip_init(struct bu27034_data *data)
{
	int ret;

	/* Reset */
	ret = regmap_update_bits(data->regmap, BU27034_REG_SYSTEM_CONTROL,
			   BU27034_MASK_SW_RESET, BU27034_MASK_SW_RESET);
	if (ret)
		return dev_err_probe(data->dev, ret, "Sensor reset failed\n");

	/*
	 * Delay to allow IC to initialize. We don't care if we delay
	 * for more than 1 ms so msleep() is Ok. We just don't want to
	 * block
	 */
	msleep(1);

	/*
	 * Consider disabling the measurement (and powering off the sensor) for
	 * runtime pm
	 */
	ret = bu27034_meas_en(data);
	if (ret)
		return ret;

	return devm_add_action_or_reset(data->dev, bu27034_meas_stop, data);
}

static int bu27034_probe(struct i2c_client *i2c)
{
	struct device *dev = &i2c->dev;
	struct fwnode_handle *fwnode;
	struct bu27034_data *data;
	struct regmap *regmap;
	struct iio_dev *idev;
	unsigned int part_id;
	int ret;

	regmap = devm_regmap_init_i2c(i2c, &bu27034_regmap);
	if (IS_ERR(regmap))
		return dev_err_probe(dev, PTR_ERR(regmap),
				     "Failed to initialize Regmap\n");

	fwnode = dev_fwnode(dev);
	if (!fwnode)
		return -ENODEV;

	idev = devm_iio_device_alloc(dev, sizeof(*data));
	if (!idev)
		return -ENOMEM;

	ret = devm_regulator_get_enable_optional(dev, "vdd");
	if (ret != -ENODEV)
		return dev_err_probe(dev, ret, "Failed to get regulator\n");

	data = iio_priv(idev);

	ret = regmap_read(regmap, BU27034_REG_SYSTEM_CONTROL, &part_id);
	if (ret)
		return dev_err_probe(dev, ret, "Failed to access sensor\n");

	part_id &= BU27034_MASK_PART_ID;

	if (part_id != BU27034_ID) {
		dev_err(dev, "unsupported device 0x%x\n", part_id);
		return -EINVAL;
	}

	ret = iio_init_iio_gts(BU27034_SCALE_1X, 0, bu27034_gains,
			       ARRAY_SIZE(bu27034_gains), bu27034_itimes,
			       ARRAY_SIZE(bu27034_itimes), &data->gts);
	if (ret)
		return ret;

	mutex_init(&data->mutex);
	data->regmap = regmap;
	data->dev = dev;

	idev->channels = bu27034_channels;
	idev->num_channels = ARRAY_SIZE(bu27034_channels);
	idev->name = "bu27034-als";
	idev->info = &bu27034_info;

	idev->modes = INDIO_DIRECT_MODE;

	ret = bu27034_chip_init(data);
	if (ret)
		return ret;

	ret = devm_iio_device_register(data->dev, idev);
	if (ret < 0)
		return dev_err_probe(dev, ret,
				     "Unable to register iio device\n");

	return ret;
}

static const struct of_device_id bu27034_of_match[] = {
	{ .compatible = "rohm,bu27034", },
	{ }
};
MODULE_DEVICE_TABLE(of, bu27034_of_match);

static struct i2c_driver bu27034_i2c_driver = {
	.driver = {
		.name  = "bu27034-i2c",
		.of_match_table = bu27034_of_match,
	  },
	.probe_new    = bu27034_probe,
};
module_i2c_driver(bu27034_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matti Vaittinen <matti.vaittinen@fi.rohmeurope.com>");
MODULE_DESCRIPTION("ROHM BU27034 ambient light sensor driver");
