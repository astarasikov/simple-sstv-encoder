#ifndef __SSTV_COMMON_HPP__
#define __SSTV_COMMON_HPP__

enum sstv_frequencies {
	SSTV_ONE = 1100,
	SSTV_ZERO = 1300,
	SSTV_SYNC = 1500,
	SSTV_RGB_MIN = 1500,
	SSTV_RGB_MAX = 2300,
	SSTV_LEAD_TONE = 1900,
	SSTV_BREAK = 1200,
};

enum sstv_timings {
	SSTV_TIME_LEAD_US = 300 * 1000,
	SSTV_TIME_BIT_US = 30 * 1000,
	SSTV_TIME_BREAK_US = 10 * 1000,
};

enum sstv_vis_code {
	MARTIN_M1 = 44,
	MARTIN_M2 = 40,
	SCOTTIE_1 = 60,
	SCOTTIE_2 = 56,
	SCOTTIE_DX = 76,
	ROBOT = 8,
	ROBOT_72 = 12,
	SC2_180 = 55,
};

struct sstv_mode_info {
	const unsigned width;
	const unsigned height;
	const char* const name;
	const enum sstv_vis_code visCode;
};

static const struct sstv_mode_info MartinM1Info = {
	320, 256, "Martin M1", MARTIN_M1
};

static const struct sstv_mode_info MartinM2Info = {
	320, 256, "Martin M2", MARTIN_M2
};

static const unsigned MARTIN_BREAK_TIME_US = 4862;
static const unsigned MARTIN_SYNC_TIME_US = 572;
static const unsigned M1_PIXEL_TIME_US = 458;
static const unsigned M2_PIXEL_TIME_US = 228;

#endif
