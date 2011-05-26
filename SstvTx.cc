#include "SstvTx.hpp"
#include "CommonFunctions.hpp"
#include <cassert>
#include <cmath>

void SstvTransmitter :: bufferTone(unsigned frequency,
	unsigned duration) {
	const unsigned one_second = 1 * 1000 * 1000;
	while (duration) {
		unsigned freqOut = soundOutput->getFrequency();
		unsigned txLength = duration > one_second ? one_second : duration; 
		
		double freq = 2.0 * M_PI * frequency;
		double phaseDelta = freq / freqOut;
		double flLength = (1.0 * duration) / one_second;

		double phaseNow = phase + sync;
		double phaseEnd = phase + freq * flLength;
		while (phaseNow < phaseEnd) {
			phaseNow += phaseDelta;
			double wave = sin(phaseNow);
			int signal = 128 + 128 * wave;
			pcmBuffer[bufferPosition++] = signal < 255 ? signal : 255;
		}
		sync = phaseNow - phaseEnd;

		while (phaseEnd > 2.0 * M_PI)
			phaseEnd -= 2.0 * M_PI;
		phase = phaseEnd;
		
		duration -= txLength;
	}
}

void SstvTransmitter :: sendVISCode(void) {
	assert(soundOutput != 0);
	bufferTone(SSTV_LEAD_TONE, SSTV_TIME_LEAD_US);
	bufferTone(SSTV_BREAK, SSTV_TIME_BREAK_US);
	bufferTone(SSTV_LEAD_TONE, SSTV_TIME_LEAD_US);
	bufferTone(SSTV_BREAK, SSTV_TIME_BIT_US);

	unsigned char vis = info.visCode;
	for (int i = 0; i < 7; i++) {
		if (vis & (1 << i)) {
			bufferTone(SSTV_ONE, SSTV_TIME_BIT_US);
		}
		else {
			bufferTone(SSTV_ZERO, SSTV_TIME_BIT_US);
		}
	}
	bool parity;
	while (vis) {
		parity = !parity;
		vis &= (vis - 1);
	}
	if (parity) {
		bufferTone(SSTV_ZERO, SSTV_TIME_BIT_US);
	}
	else {
		bufferTone(SSTV_ONE, SSTV_TIME_BIT_US);
	}

	bufferTone(SSTV_BREAK, SSTV_TIME_BIT_US);
}

MartinM1Transmitter :: MartinM1Transmitter(SoundPlayer* _soundOutput) :
	MartinTransmitter(MartinM1Info, _soundOutput) {

	pixelTime = M1_PIXEL_TIME_US;
	bufferSize = 147 * _soundOutput->getFrequency();
	pcmBuffer = new unsigned char[bufferSize];
}

MartinM1Transmitter :: ~MartinM1Transmitter() {
	delete[] pcmBuffer;
}

MartinM2Transmitter :: MartinM2Transmitter(SoundPlayer* _soundOutput) :
	MartinTransmitter(MartinM2Info, _soundOutput) {

	pixelTime = M2_PIXEL_TIME_US;
	bufferSize = 59 * _soundOutput->getFrequency();
	pcmBuffer = new unsigned char[bufferSize];
}

MartinM2Transmitter :: ~MartinM2Transmitter() {
	delete[] pcmBuffer;
}

void MartinTransmitter :: transmit(unsigned char* imageData) {
	bufferPosition = 0;
	phase = 0;
	sync = 1.0 / soundOutput->getFrequency();
	unsigned range = SSTV_RGB_MAX - SSTV_RGB_MIN;
	assert(soundOutput != 0);
	assert(imageData != 0);
	sendVISCode();
	for (unsigned i = 0; i < info.height; i++) {
		//sync pulse
		bufferTone(SSTV_BREAK, MARTIN_BREAK_TIME_US);
		//sync porch
		bufferTone(SSTV_SYNC, MARTIN_SYNC_TIME_US);
		
		//send green, blue, red scanlines
		for (unsigned k = 0; k < 3; k++) {
			int z = k == 2 ? k : 1 ^ k;
			for (unsigned j = 0; j < info.width; j++) {
				unsigned char val = imageData[3*i*info.width + 3*j + z];
				bufferTone(SSTV_RGB_MIN + ((range * val) >> 8), pixelTime);
			}
			//line separator
			bufferTone(SSTV_SYNC, MARTIN_SYNC_TIME_US);
		}
	}
	
	soundOutput->transmitData(pcmBuffer, bufferPosition);
}
