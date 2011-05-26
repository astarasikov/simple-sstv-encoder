#ifndef __SSTV_CODER_HPP__
#define __SSTV_CODER_HPP__

#include "SstvCommon.hpp"
#include "SoundBackend.hpp"
#include "DSPFunctions.hpp"

class SstvReceiver {
public:
	SstvReceiver(const struct sstv_mode_info &_info, Detector *_detector)
		: info(_info),
		detector(_detector) {}
	virtual ~SstvReceiver() {}
	virtual void receive(unsigned char* outputBuffer) = 0;

	const struct sstv_mode_info &info;
protected:
	Detector *detector;
};

class MartinM1Receiver : public SstvReceiver {
public:
	MartinM1Receiver(Detector *_detector)
	: SstvReceiver(MartinM1Info, _detector) {}
	~MartinM1Receiver() {}
	void receive(unsigned char* outputBuffer);
};

#endif
