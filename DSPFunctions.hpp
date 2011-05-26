#ifndef __DSP_FUNCTIONS_HPP__
#define __DSP_FUNCTIONS_HPP__

#include "SoundBackend.hpp"

class Detector {
public:
	struct Response {
		unsigned frequency;
		unsigned duration;
	};
	Detector(SoundRecorder *_soundInput,
		unsigned bufferSeconds)
		:soundInput(_soundInput),
		bufferSize(10 * bufferSeconds * _soundInput->getFrequency()),
		pcmBuffer(new unsigned char[bufferSize]) {}
	virtual ~Detector() {}
	virtual Response waitForTone(unsigned tone, unsigned bandwidth,
		unsigned minDuration) = 0;
protected:
	SoundRecorder *soundInput;

	unsigned bufferSize;
	unsigned char* pcmBuffer;
};

class ZCDetector : public Detector {
public:
	ZCDetector(SoundRecorder *_soundInput, unsigned bufferSeconds)
	: Detector(_soundInput, bufferSeconds) {}
	virtual ~ZCDetector() {}
	Response waitForTone(unsigned tone, unsigned bandwidth,
		unsigned minDuration);
protected:
	bool isRunning;
};

#endif
