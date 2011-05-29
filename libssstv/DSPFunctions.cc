#include "DSPFunctions.hpp"
#include "CommonFunctions.hpp"

#include <cstdlib>
#include <cmath>

Detector::Response FMDetector :: waitForTone(unsigned tone, unsigned bandwidth,
unsigned minDuration) {
	struct Response resp;
	unsigned freqOut = soundInput->getFrequency();
	const double band = 20;
	
	//multiplier to convert phase to frequency
	double freqMult = freqOut / (2.0 * M_PI);
	while (1) {
		int nframes = soundInput->receiveData(pcmBuffer, freqOut);
		for (int i = 1; i < nframes; i++) {
			if (pcmBuffer[i] == 128 || pcmBuffer[i] == pcmBuffer[i - 1])
				continue;

			double dcur = (pcmBuffer[i] - pcmBuffer[i - 1]) / 128.0;
		
			//if we have a diff more than 1 in absolute value,
			//that usually means the frequency has changed
			if (dcur < -1 || dcur > 1)
				continue;

			dcur = freqMult * fabs(asin(dcur));

			double dfreq = dcur * freqOut;
			
			//This is kinda wrong and a proper band-pass filter
			if (dfreq < 1050 || dfreq > 2350)
				continue;

			Dprintf("[%d] Delta=%f, freq=%f\n", i, dcur, dfreq);
			double dlast = dfreq;
			int frames = 0;
			while (++i < nframes ) {
				double d0 = (pcmBuffer[i] - pcmBuffer[i - 1]) / 128.0;
				if (d0 < -1 || d0 > 1)
					continue;
				double df0 = freqMult * fabs(asin(d0));
				if (fabs(df0 - dfreq) < band) {
					++frames;
				}
				else {
					double duration = (++frames * 1.0) / freqOut;
					Dprintf("%d consequent frames, duration=%f\n",
						frames, duration);
					//The continuous period of the same frequency has ended
					//so we roll one sample back to force the loop to run from
					//start
					--i;
					break;
				}
			}
		}
	}
	return resp;
}

