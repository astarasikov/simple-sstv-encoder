#include "DSPFunctions.hpp"
#include "CommonFunctions.hpp"

#include <cstdlib>
#include <cmath>

Detector::Response FMDetector :: waitForTone(unsigned tone, unsigned bandwidth,
unsigned minDuration) {
	struct Response resp;
	unsigned freqOut = soundInput->getFrequency();
	const double band = 20;
	while (1) {
		double davg = 0;
		int iavg = 0;
		int nframes = soundInput->receiveData(pcmBuffer, freqOut);
		for (int i = 1; i < nframes; i++) {
			//if (pcmBuffer[i - 1] != 128 || pcmBuffer[i] == 128)
			//	continue;
			if (pcmBuffer[i] == 128 || pcmBuffer[i] == pcmBuffer[i - 1])
				continue;

			double dcur = (pcmBuffer[i] - pcmBuffer[i - 1]) / 128.0;
			
			if (dcur < -1 || dcur > 1)
				continue;

			dcur = asin(dcur);
			dcur = fabs(dcur);
			dcur /= (2.0 * M_PI);

			double dfreq = dcur * freqOut;
			if (dfreq < 1050 || dfreq > 2350)
				continue;
			davg += dfreq;
			++iavg;

			Dprintf("[%d] Delta=%f, freq=%f\n", i, dcur, dfreq);
			double dlast = dfreq;
			int frames = 0;
			while (++i < nframes ) {
				double d0 = (pcmBuffer[i] - pcmBuffer[i - 1]) / 128.0;
				if (d0 < -1 || d0 > 1)
					continue;
				double df0 = fabs(asin(d0)) / (2.0 * M_PI);
				df0 *= freqOut;
				if (fabs(df0 - dfreq) < band) {
					++frames;
					double duration = (++frames * 1.0) / freqOut;
				}
				else {
					double duration = (++frames * 1.0) / freqOut;
					Dprintf("%d consequent frames, duration=%f\n",
						frames, duration);
					--i;
					break;
				}
			}
		}
		davg /= iavg;
		Dprintf("Average frequency=%f\n", davg);
	}
	return resp;
}

