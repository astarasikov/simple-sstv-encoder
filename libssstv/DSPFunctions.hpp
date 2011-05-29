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
		bufferSize(3 * bufferSeconds * _soundInput->getFrequency()),
		pcmBuffer(new unsigned char[bufferSize]) {}
	virtual ~Detector() {}
	virtual Response waitForTone(unsigned tone, unsigned bandwidth,
		unsigned minDuration) = 0;
protected:
	SoundRecorder *soundInput;

	unsigned bufferSize;
	unsigned char* pcmBuffer;
};

class FMDetector : public Detector {
public:
	FMDetector(SoundRecorder *_soundInput, unsigned bufferSeconds)
	: Detector(_soundInput, bufferSeconds) {}
	virtual ~FMDetector() {}
	Response waitForTone(unsigned tone, unsigned bandwidth,
		unsigned minDuration);
protected:
	bool isRunning;
};

typedef Detector::Response FmResponse;

#endif
