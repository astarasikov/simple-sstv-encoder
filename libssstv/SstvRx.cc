#include "SstvRx.hpp"
#include "CommonFunctions.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>

//first goes green, then red
enum state {
	SSTV_OFF,
	WAIT_FOR_VIS,
	WAIT_FOR_BREAK,
	WAIT_FOR_SYNC,
	SCAN_GREEN,
	SCAN_RED,
	SCAN_BLUE,
} state = SSTV_OFF;

unsigned lastFreq = 0, lastLength = 0, pixel = 0;

void MartinM1Receiver :: receive(unsigned char* outputBuffer) {
	Dprintf("+%s\n", __func__);
	Dprintf("-%s\n", __func__);
}
