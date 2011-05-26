#ifndef __SOUND_SNDFILE_HPP__
#define __SOUND_SNDFILE_HPP__

#include "SoundBackend.hpp"

class SoundSndfilePlayerImpl;
class SoundSndfileRecorderImpl;

class SoundSndfilePlayer : public SoundPlayer {
public:
	SoundSndfilePlayer(char *filename,
		unsigned bufferCount, unsigned frequency);
	~SoundSndfilePlayer();
	virtual bool transmitData(unsigned char* bufferData, unsigned bufferSize);
private:
	SoundSndfilePlayerImpl *impl;
};

class SoundSndfileRecorder : public SoundRecorder {
public:
	SoundSndfileRecorder(char *filename, unsigned frequency);
	~SoundSndfileRecorder();
	virtual unsigned receiveData(unsigned char *buffer, unsigned sampleCount);
private:
	SoundSndfileRecorderImpl *impl;
};
#endif
