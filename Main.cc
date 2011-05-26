#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "CommonFunctions.hpp"
#include "ImageFunctions.hpp"
#include "SoundBackend.hpp"
#include "SoundOpenAl.hpp"
#include "SoundSndfile.hpp"
#include "SstvRx.hpp"
#include "SstvTx.hpp"
#include "DSPFunctions.hpp"

#define FALLBACK_FREQ 8000

static void sstv_receive(char* fname_in, char* fname_out,
	char* mode, char* audio_drv,
	unsigned freq) {
	Dprintf("+%s %s %s %s %s %u\n", __func__, fname_in, fname_out,
		mode, audio_drv, freq);

	if (!fname_out) {
		puts(_("Please specify the output file name"));
		return;
	}
	if (!audio_drv) {
		puts(_("Please specify the audio driver"));
		return;
	}

	SoundRecorder *snd;

	if (!strcmp(audio_drv, "sndfile")) {
		if (!fname_in) {
			puts(_("Please specify the input file name"));
			return;
		}
		snd = new SoundSndfileRecorder(fname_in, freq);
	}
	else if (!strcmp(audio_drv, "openal")) {
		snd = new SoundOpenAlRecorder(freq);
	}
	else {
		puts(_("Please specify one of the supported sound drivers\n"));
		return;
	}
	ZCDetector detector(snd, 1);
	MartinM1Receiver *rcvr = new MartinM1Receiver(&detector);
	unsigned char *data =
		new unsigned char[rcvr->info.width * rcvr->info.height * 3];
	rcvr->receive(data);

	save_rgb(data, rcvr->info.width, rcvr->info.height, fname_out);
	delete[] data;
	delete rcvr;
	delete snd;
	Dprintf("-%s\n", __func__);
}

static void sstv_transmit(char* fname_in, char* fname_out,
	char* mode, char* audio_drv,
	unsigned freq) {
	Dprintf("+%s %s %s %s %s %u\n", __func__, fname_in, fname_out,
		mode, audio_drv, freq);
	if (!fname_in) {
		puts(_("Please specify the input file name"));
		return;
	}
	if (!audio_drv) {
		puts(_("Please specify the audio driver"));
		return;
	}
	SoundPlayer *snd;

	if (!strcmp(audio_drv, "sndfile")) {
		if (!fname_in) {
			puts(_("Please specify the output file name"));
			return;
		}
		snd = new SoundSndfilePlayer(fname_out, 1, freq); 
	}
	else if (!strcmp(audio_drv, "openal")) {
		snd = new SoundOpenAlPlayer(1, freq);
	}
	else {
		puts(_("Please specify one of the supported sound drivers"));
		return;
	}
	unsigned char* data;
	int ret = load_rgb(&data, fname_in, MartinM1Info.width, MartinM1Info.height);

	if (!data) {
		printf(_("Unable to open the image file, error code %d\n"), ret);
	}
	else {
		MartinM1Transmitter xmit(snd);
		xmit.transmit(data);
		delete[] data;
	}

	delete snd;
	Dprintf("-%s\n", __func__);
}

static void display_help(void) {
	printf(_("Usage: ssstv-cli -r|-t [OPTIONS...]\n"));
	printf(_("ssstv-cli - A simple SSTV decoder\n"));
	printf(_("Options:\n"));
	printf(_("\t-t\t transmit (encode) the image\n"));
	printf(_("\t-r\t receive (decode) the image\n"));
	printf(_("\t-i\t input file name if applicable\n"));
	printf(_("\t-o\t output file name if applicable\n"));
	printf(_("\t-m\t sstv mode\n"));
	printf(_("\t-f\t pcm rate (frequency)\n"));
	//printf(_("\t-s\t image scaling mode. Possible modes are:\n"
	//	"\t\thclip - clip extra space on right and left\n"
	//	"\t\tvclip - clip extra space on top and bottom\n"
	//	"\t\tsqueeze - scale ignoring proportions\n"
	//));
	printf(_("\t-a\t audio driver or sound backend to use\n"));
	printf(_("\t-l\t list available options for some arguements\n"
		"\t\td or decoders - show available SSTV decoders\n"
		"\t\te or encoders - show available SSTV encoders\n"
		"\t\tr or recorders - show available audio drivers for transmit mode\n"
		"\t\tp or players - show available audio drivers for receive mode\n"
		"\t\tf or formats - show available audio formats for the driver used\n"
	));
}

//TODO: query available modules at run-time
static void list_objects(char* type) {
	if (!type || !type[0]) {
		printf(_("Please specify the type of objects to list\n"));
	}
	switch (type[0]) {
		case 'd':
			printf(_("martinm1 - Martin M1 encoder\n"));
			break;
		case 'e':
			printf(_("martinm1 - Martin M1 decoder\n"));
			break;
		case 'r':
			printf(_("8000 - use 8-bit unsigned mono at 8000Hz\n"
			"44100 - use 8-bit unsigned mono at 44100Hz\n"
			));
			break;
		case 'p':
		case 'f':
			printf(_(
				"openal - use OpenAl backend to access audio hardware\n"
				"sndfile - use libsndfile to use only file-based io\n"
			));
			break;
	}
}

static void handle_cli(int argc, char** argv) {
	int opt;
	enum ssstv_action {
		NONE,
		TX,
		RX,
	} action = NONE;

	unsigned freq = FALLBACK_FREQ;
	char *fname_in = 0, *fname_out = 0, *mode = 0, *audio_drv = 0;
	char *err_action = _("Please use only one of -t, -r or -l");

	while ((opt = getopt(argc, argv, "trf:a:m:l:hi:o:")) != -1) {
		switch(opt) {
			case 't':
				if (action != NONE) {
					puts(err_action);
					return;
				}
				action = TX;
				break;
			case 'r':
				if (action != NONE) {
					puts(err_action);
					return;
				}
				action = RX;
				break;
			case 'o':
				fname_out = optarg;
				break;
			case 'i':
				fname_in = optarg;
				break;
			case 'm':
				mode = optarg;
				break;
			case 'a':
				audio_drv = optarg;
				break;
			case 'f':
				freq = atoi(optarg);
				break;
			case 'l':
				if (action != NONE) {
					puts(err_action);
				}
				else {
					list_objects(optarg);
				}
				return;
			case 'h':
				display_help();
				return;
		}
	}
	switch (action) {
		case NONE:
			break;
		case TX:
			sstv_transmit(fname_in, fname_out, mode, audio_drv, freq);
			break;
		case RX:
			sstv_receive(fname_in, fname_out, mode, audio_drv, freq);
			break;
	}
}

int main(int argc, char** argv) {
	if (argc > 1) {
#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
#endif
		handle_cli(argc, argv);
#ifdef FREEIMAGE_LIB
		FreeImage_Deinitialise();
#endif
	}
	else {
		display_help();
	}
}
