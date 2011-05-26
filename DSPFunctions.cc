#include "DSPFunctions.hpp"
#include "CommonFunctions.hpp"

#include <cstdlib>

static unsigned idx_c = 0, idx_p = 0;

Detector::Response ZCDetector :: waitForTone(unsigned tone, unsigned bandwidth,
	unsigned minDuration) {
	Dprintf("+%s\n", __func__);
	Dprintf("-%s\n", __func__);
	Response resp;
	return resp;
}

