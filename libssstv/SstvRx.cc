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

void MartinM1Receiver :: receive(unsigned char* outputBuffer) {
	Dprintf("+%s\n", __func__);
	FmResponse resp;
	
	unsigned row = 0, col = 0;

	while (1) {
		resp = detector->waitForTone(1200, 25, 1000 * 1);
		Dprintf("received freq=%u duration=%u\n", resp.frequency,
			resp.duration);
	}

	Dprintf("-%s\n", __func__);
}
