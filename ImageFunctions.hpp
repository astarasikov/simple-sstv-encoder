#ifndef __IMAGE_FUNCTIONS_HPP__
#define __IMAGE_FUNCTIONS_HPP__

int load_rgb(unsigned char **buffer, char *filename,
	unsigned width, unsigned height);
int save_rgb(unsigned char* buffer, unsigned width, unsigned height,
	char* filename);

#endif
