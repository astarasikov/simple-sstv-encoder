#include "SoundSndfile.hpp"
#include "CommonFunctions.hpp"

#include <sndfile.h>

#include <cmath>

class SoundSndfileRecorderImpl {
public:
	SoundSndfileRecorderImpl(char* fname, unsigned frequency) {
		info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_U8;
		info.samplerate = frequency;
		file = sf_open(fname, SFM_READ, &info);

		//TODO: throw an exception here
		if (sf_error(0)) {
			Dprintf("%s %s\n", __func__, sf_strerror(0));
		}
		else {
			Dprintf("%s file=%08x\n", __func__,
				reinterpret_cast<unsigned>(file));
		}
	}
	~SoundSndfileRecorderImpl() {
		sf_close(file);
	}

	SNDFILE* file;
	SF_INFO info;
};

SoundSndfileRecorder :: SoundSndfileRecorder(char* fname, unsigned frequency)
	: SoundRecorder(frequency),
	impl(new SoundSndfileRecorderImpl(fname, frequency)) {
}

SoundSndfileRecorder :: ~SoundSndfileRecorder() {
	delete impl;
}

unsigned SoundSndfileRecorder :: receiveData(unsigned char *destBuffer,
	unsigned sampleCount) {
	return sf_read_raw(impl->file, destBuffer, sampleCount);
}

class SoundSndfilePlayerImpl {
public:
	SoundSndfilePlayerImpl(char* fname,
		unsigned _bufferCount, unsigned _frequency){
		info.samplerate = _frequency;
		info.frames = _bufferCount;
		info.channels = 1;
		info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_U8;
		file = sf_open(fname, SFM_WRITE, &info);

		//TODO: throw an exception here
		if (sf_error(0)) {
			Dprintf("err %s\n", sf_strerror(0));
		}
	}
	~SoundSndfilePlayerImpl() {
		sf_close(file);
	}
	SNDFILE* file;
	SF_INFO info;
};

SoundSndfilePlayer :: SoundSndfilePlayer(char* fname,
	unsigned bufferCount, unsigned frequency)
	: SoundPlayer(bufferCount, frequency),
	impl(new SoundSndfilePlayerImpl(fname, bufferCount, frequency)) {
}

SoundSndfilePlayer :: ~SoundSndfilePlayer() {
	delete impl;
}

bool SoundSndfilePlayer :: transmitData(unsigned char* sstvBuffer,
	unsigned sstvBufferSize) {
	Dprintf("+%s\n", __func__);
	sf_write_raw(impl->file, sstvBuffer, sstvBufferSize);
	Dprintf("-%s\n", __func__);
	return true;
}
