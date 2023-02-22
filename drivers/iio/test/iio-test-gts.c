// SPDX-License-Identifier: GPL-2.0-only
/* Unit tests for IIO light sensor gain-time-scale helpers
 *
 * Copyright (c) 2023 Matti Vaittinen <mazziesaccount@gmail.com>
 */

#include <kunit/test.h>
#include "../light/gain-time-scale-helper.h"

/*
 * Please, read the "rant" from the top of the lib/test_linear_ranges.c if
 * you see a line of helper code which is not being tested.
 *
 * Then, please look at the line which is not being tested. Is this line
 * somehow unusually complex? If answer is "no", then chances are that the
 * "development inertia" caused by adding a test exceeds the benefits.
 *
 * If yes, then adding a test is probably a good idea but please stop for a
 * moment and consider the effort of changing all the tests when code gets
 * refactored. Eventually it neeeds to be.
 */

#define TEST_TSEL_50		1
#define TEST_TSEL_X_MIN		TEST_TSEL_50
#define TEST_TSEL_100		0
#define TEST_TSEL_200		2
#define TEST_TSEL_400		4
#define TEST_TSEL_X_MAX		TEST_TSEL_400

#define TEST_GSEL_1		0x00
#define TEST_GSEL_X_MIN		TEST_GSEL_1
#define TEST_GSEL_4		0x08
#define TEST_GSEL_16		0x0a
#define TEST_GSEL_32		0x0b
#define TEST_GSEL_64		0x0c
#define TEST_GSEL_256		0x18
#define TEST_GSEL_512		0x19
#define TEST_GSEL_1024		0x1a
#define TEST_GSEL_2048		0x1b
#define TEST_GSEL_4096		0x1c
#define TEST_GSEL_X_MAX		TEST_GSEL_4096

#define TEST_SCALE_1X		64
#define TEST_SCALE_MIN_X	TEST_SCALE_1X
#define TEST_SCALE_2X		32
#define TEST_SCALE_4X		16
#define TEST_SCALE_8X		8
#define TEST_SCALE_16X		4
#define TEST_SCALE_32X		2
#define TEST_SCALE_64X		1

#define TEST_SCALE_NANO_128X	500000000
#define TEST_SCALE_NANO_256X	250000000
#define TEST_SCALE_NANO_512X	125000000
#define TEST_SCALE_NANO_1024X	62500000
#define TEST_SCALE_NANO_2048X	31250000
#define TEST_SCALE_NANO_4096X	15625000
#define TEST_SCALE_NANO_4096X2	7812500
#define TEST_SCALE_NANO_4096X4	3906250
#define TEST_SCALE_NANO_4096X8	1953125

#define TEST_SCALE_NANO_MAX_X TEST_SCALE_NANO_4096X8

static const struct iio_gain_sel_pair gts_test_gains[] = {
	GAIN_SCALE_GAIN(1, TEST_GSEL_1),
	GAIN_SCALE_GAIN(4, TEST_GSEL_4),
	GAIN_SCALE_GAIN(16, TEST_GSEL_16),
	GAIN_SCALE_GAIN(32, TEST_GSEL_32),
	GAIN_SCALE_GAIN(64, TEST_GSEL_64),
	GAIN_SCALE_GAIN(256, TEST_GSEL_256),
	GAIN_SCALE_GAIN(512, TEST_GSEL_512),
	GAIN_SCALE_GAIN(1024, TEST_GSEL_1024),
	GAIN_SCALE_GAIN(2048, TEST_GSEL_2048),
	GAIN_SCALE_GAIN(4096, TEST_GSEL_4096),
#define HWGAIN_MAX 4096
};

static const struct iio_itime_sel_mul gts_test_itimes[] = {
	GAIN_SCALE_ITIME_MS(400, TEST_TSEL_400, 8),
	GAIN_SCALE_ITIME_MS(200, TEST_TSEL_200, 4),
	GAIN_SCALE_ITIME_MS(100, TEST_TSEL_100, 2),
	GAIN_SCALE_ITIME_MS(50, TEST_TSEL_50, 1),
#define TIMEGAIN_MAX 8
};
#define TOTAL_GAIN_MAX	(HWGAIN_MAX * TIMEGAIN_MAX)

static int test_init_iio_gain_scale(struct iio_gts *gts, int max_scale_int,
				int max_scale_nano)
{
	int ret;

	ret = iio_init_iio_gts(max_scale_int, max_scale_nano, gts_test_gains,
			       ARRAY_SIZE(gts_test_gains), gts_test_itimes,
			       ARRAY_SIZE(gts_test_itimes), gts);

	return ret;
}

static void test_iio_gts_find_gain_for_scale_using_time(struct kunit *test)
{
	struct iio_gts gts;
	int ret, gain;

	ret = test_init_iio_gain_scale(&gts, TEST_SCALE_1X, 0);
	KUNIT_EXPECT_EQ(test, 0, ret);
	if (ret)
		return;

	ret = iio_gts_find_gain_for_scale_using_time(&gts, TEST_TSEL_100,
						     TEST_SCALE_8X, 0, &gain);
	/*
	 * Meas time 100 => gain by time 2x
	 * TEST_SCALE_8X matches total gain 8x
	 * => required HWGAIN 4x
	 */
	KUNIT_EXPECT_EQ(test, 0, ret);
	KUNIT_EXPECT_EQ(test, 4, gain);

	ret = iio_gts_find_gain_for_scale_using_time(&gts, TEST_TSEL_200, 0,
						TEST_SCALE_NANO_256X, &gain);
	/*
	 * Meas time 200 => gain by time 4x
	 * TEST_SCALE_256X matches total gain 256x
	 * => required HWGAIN 256/4 => 64x
	 */
	KUNIT_EXPECT_EQ(test, 0, ret);
	KUNIT_EXPECT_EQ(test, 64, gain);

	/* Min time, Min gain */
	ret = iio_gts_find_gain_for_scale_using_time(&gts, TEST_TSEL_X_MIN,
						TEST_SCALE_MIN_X, 0, &gain);
	KUNIT_EXPECT_EQ(test, 0, ret);
	KUNIT_EXPECT_EQ(test, 1, gain);

	/* Max time, Max gain */
	ret = iio_gts_find_gain_for_scale_using_time(&gts, TEST_TSEL_X_MAX, 0,
						TEST_SCALE_NANO_MAX_X, &gain);
	KUNIT_EXPECT_EQ(test, 0, ret);
	KUNIT_EXPECT_EQ(test, 4096, gain);

	ret = iio_gts_find_gain_for_scale_using_time(&gts, TEST_TSEL_100, 0,
						TEST_SCALE_NANO_256X, &gain);
	/*
	 * Meas time 100 => gain by time 2x
	 * TEST_SCALE_256X matches total gain 256x
	 * => required HWGAIN 256/2 => 128x (not in gain-table - unsupported)
	 */
	KUNIT_EXPECT_NE(test, 0, ret);

	ret = iio_gts_find_gain_for_scale_using_time(&gts, TEST_TSEL_200, 0,
						TEST_SCALE_NANO_MAX_X, &gain);
	/* We can't reach the max gain with integration time smaller than MAX */
	KUNIT_EXPECT_NE(test, 0, ret);

	ret = iio_gts_find_gain_for_scale_using_time(&gts, TEST_TSEL_50, 0,
						TEST_SCALE_NANO_MAX_X, &gain);
	/* We can't reach the max gain with integration time smaller than MAX */
	KUNIT_EXPECT_NE(test, 0, ret);
}

static void test_iio_gts_find_time_and_gain_sel_for_scale(struct kunit *test)
{
	struct iio_gts gts;
	int ret, gain_sel, time_sel;

	ret = test_init_iio_gain_scale(&gts, TEST_SCALE_1X, 0);
	KUNIT_EXPECT_EQ(test, 0, ret);
	if (ret)
		return;

	ret = iio_gts_find_time_and_gain_sel_for_scale(&gts, 0,
			TEST_SCALE_NANO_256X, &gain_sel, &time_sel);
	/*
	 * We should find time 400 (8x) and gain 256/8 => 32x because the
	 * time 400 is listed first
	 */
	KUNIT_EXPECT_EQ(test, 0, ret);
	KUNIT_EXPECT_EQ(test, TEST_GSEL_32, gain_sel);
	KUNIT_EXPECT_EQ(test, TEST_TSEL_400, time_sel);

	ret = iio_gts_find_time_and_gain_sel_for_scale(&gts, TEST_SCALE_64X,
						       0, &gain_sel, &time_sel);
	/*
	 * We should find time 200 (4x) and gain 64/4 => 16x. The most
	 * preferred time 400 (8x) would require gain 8x - which is not
	 * "supported".
	 */
	KUNIT_EXPECT_EQ(test, 0, ret);
	KUNIT_EXPECT_EQ(test, TEST_GSEL_16, gain_sel);
	KUNIT_EXPECT_EQ(test, TEST_TSEL_200, time_sel);

	/* Min gain */
	ret = iio_gts_find_time_and_gain_sel_for_scale(&gts, TEST_SCALE_MIN_X,
						0, &gain_sel, &time_sel);
	/*
	 * We should find time 400 (8x) and gain 256/8 => 32x because the
	 * time 400 is listed first
	 */
	KUNIT_EXPECT_EQ(test, 0, ret);
	KUNIT_EXPECT_EQ(test, TEST_GSEL_1, gain_sel);
	KUNIT_EXPECT_EQ(test, TEST_TSEL_50, time_sel);

	/* Max gain */
	ret = iio_gts_find_time_and_gain_sel_for_scale(&gts, 0,
			TEST_SCALE_NANO_MAX_X, &gain_sel, &time_sel);
	/*
	 * We should find time 400 (8x) and gain 256/8 => 32x because the
	 * time 400 is listed first
	 */
	KUNIT_EXPECT_EQ(test, 0, ret);
	KUNIT_EXPECT_EQ(test, TEST_GSEL_X_MAX, gain_sel);
	KUNIT_EXPECT_EQ(test, TEST_TSEL_X_MAX, time_sel);
}

static void test_iio_gts_get_total_gain_by_sel(struct kunit *test)
{
	struct iio_gts gts;
	int ret, gain_sel, time_sel;

	ret = test_init_iio_gain_scale(&gts, TEST_SCALE_1X, 0);
	KUNIT_EXPECT_EQ(test, 0, ret);
	if (ret)
		return;

	/* gain x32, time x4 => total gain 32 * 4 = 128 */
	gain_sel = TEST_GSEL_32;
	time_sel = TEST_TSEL_200;

	ret = iio_gts_get_total_gain_by_sel(&gts, gain_sel, time_sel);
	/* gain x32, time x8 => total gain 32 * 4 = 128 */
	KUNIT_EXPECT_EQ(test, 128, ret);

	gain_sel = TEST_GSEL_X_MAX;
	time_sel = TEST_TSEL_X_MAX;
	ret = iio_gts_get_total_gain_by_sel(&gts, gain_sel, time_sel);
	KUNIT_EXPECT_EQ(test, TOTAL_GAIN_MAX, ret);

	gain_sel = TEST_GSEL_X_MIN;
	time_sel = TEST_TSEL_X_MIN;
	ret = iio_gts_get_total_gain_by_sel(&gts, gain_sel, time_sel);
	KUNIT_EXPECT_EQ(test, 1, ret);
}

static void test_iio_gts_find_new_gain_sel_by_old_gain_time(struct kunit *test)
{
	struct iio_gts gts;
	int ret, old_gain, new_gain, old_time_sel, new_time_sel;

	ret = test_init_iio_gain_scale(&gts, TEST_SCALE_1X, 0);
	KUNIT_EXPECT_EQ(test, 0, ret);
	if (ret)
		return;

	old_gain = 32;
	old_time_sel = TEST_TSEL_200;
	new_time_sel = TEST_TSEL_400;

	ret = iio_gts_find_new_gain_sel_by_old_gain_time(&gts, old_gain,
					old_time_sel, new_time_sel, &new_gain);
	KUNIT_EXPECT_EQ(test, 0, ret);
	/*
	 * Doubling the integration time doubles the total gain - so old
	 * (hw)gain must be divided by two to compensate. => 32 / 2 => 16
	 */
	KUNIT_EXPECT_EQ(test, 16, new_gain);

	old_gain = 4;
	old_time_sel = TEST_TSEL_50;
	new_time_sel = TEST_TSEL_200;
	ret = iio_gts_find_new_gain_sel_by_old_gain_time(&gts, old_gain,
					old_time_sel, new_time_sel, &new_gain);
	KUNIT_EXPECT_EQ(test, 0, ret);
	/*
	 * gain by time 1x => 4x - (hw)gain 4x => 1x
	 */
	KUNIT_EXPECT_EQ(test, 1, new_gain);

	old_gain = 512;
	old_time_sel = TEST_TSEL_400;
	new_time_sel = TEST_TSEL_50;
	ret = iio_gts_find_new_gain_sel_by_old_gain_time(&gts, old_gain,
					old_time_sel, new_time_sel, &new_gain);
	KUNIT_EXPECT_EQ(test, 0, ret);
	/*
	 * gain by time 8x => 1x - (hw)gain 512x => 4096x)
	 */
	KUNIT_EXPECT_EQ(test, 4096, new_gain);

	/* Unsupported gain 2x */
	old_gain = 4;
	old_time_sel = TEST_TSEL_200;
	new_time_sel = TEST_TSEL_400;
	ret = iio_gts_find_new_gain_sel_by_old_gain_time(&gts, old_gain,
					old_time_sel, new_time_sel, &new_gain);
	KUNIT_EXPECT_NE(test, 0, ret);

	/* Too small gain */
	old_gain = 4;
	old_time_sel = TEST_TSEL_50;
	new_time_sel = TEST_TSEL_400;
	ret = iio_gts_find_new_gain_sel_by_old_gain_time(&gts, old_gain,
					old_time_sel, new_time_sel, &new_gain);
	KUNIT_EXPECT_NE(test, 0, ret);

	/* Too big gain */
	old_gain = 1024;
	old_time_sel = TEST_TSEL_400;
	new_time_sel = TEST_TSEL_50;
	ret = iio_gts_find_new_gain_sel_by_old_gain_time(&gts, old_gain,
					old_time_sel, new_time_sel, &new_gain);
	KUNIT_EXPECT_NE(test, 0, ret);
}

static struct kunit_case iio_gts_test_cases[] = {
		KUNIT_CASE(test_iio_gts_find_gain_for_scale_using_time),
		KUNIT_CASE(test_iio_gts_find_time_and_gain_sel_for_scale),
		KUNIT_CASE(test_iio_gts_get_total_gain_by_sel),
		KUNIT_CASE(test_iio_gts_find_new_gain_sel_by_old_gain_time),
		{}
};

static struct kunit_suite iio_gts_test_suite = {
	.name = "iio-gain-time-scale",
	.test_cases = iio_gts_test_cases,
};

kunit_test_suite(iio_gts_test_suite);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matti Vaittinen <mazziesaccount@gmail.com>");
MODULE_DESCRIPTION("Test IIO light sensor gain-time-scale helpers");
