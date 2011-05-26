#ifndef __SSTV_TX_HPP__
#define __SSTV_TX_HPP__

#include "SstvCommon.hpp"
#include "SoundBackend.hpp"

class SstvTransmitter {
public:
	SstvTransmitter(const struct sstv_mode_info &_info,
		SoundPlayer* _soundOutput)
		: info(_info),
		bufferPosition(0),
		soundOutput(_soundOutput) {}
	virtual ~SstvTransmitter() {}
	virtual void transmit(unsigned char* imageData) = 0;

	const struct sstv_mode_info &info;
protected:
	virtual void sendVISCode(void);
	virtual void bufferTone(unsigned frequency, unsigned duration);

	double phase;
	double sync;
	unsigned bufferPosition;
	unsigned bufferSize;
	unsigned char* pcmBuffer;
	SoundPlayer *soundOutput;
};

class MartinTransmitter : public SstvTransmitter {
public:
	virtual void transmit(unsigned char* imageData);
protected:
	MartinTransmitter(const struct sstv_mode_info &info,
		SoundPlayer* _soundOutput) : SstvTransmitter(info, _soundOutput) {};
	~MartinTransmitter() {};
	unsigned pixelTime;
};

class MartinM1Transmitter : public MartinTransmitter {
public:
	MartinM1Transmitter(SoundPlayer* _soundOutput);
	~MartinM1Transmitter();
};

class MartinM2Transmitter : public MartinTransmitter {
public:
	MartinM2Transmitter(SoundPlayer* _soundOutput);
	~MartinM2Transmitter();
};
#endif
