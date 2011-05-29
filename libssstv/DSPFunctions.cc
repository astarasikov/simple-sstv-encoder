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

			double dfreq = (pcmBuffer[i] - pcmBuffer[i - 1]) / 128.0;
		
			//if we have a diff more than 1 in absolute value,
			//that usually means the frequency has changed
			if (dfreq < -1 || dfreq > 1)
				continue;

			dfreq = freqMult * fabs(asin(dfreq));

			//This is kinda wrong and a proper band-pass filter should be used
			if (dfreq < 1050 || dfreq > 2350)
				continue;
			Dprintf("[%i] frequency=%f\n", i, dfreq);
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
					unsigned duration = (++frames * 1000 * 1000) / freqOut;
					Dprintf("%d consequent frames, duration=%f\n",
						frames, duration);
					//The continuous period of the same frequency has ended
					//so we roll one sample back to force the loop to run from
					//start
					//
					//Oh well. let us just return instead and think of
					//how to make detection and recording asynchronous
					--i;
					resp.frequency = dfreq;
					resp.duration = duration;
					return resp;
				}
			}
		}
	}
	return resp;
}

