#include "CommonFunctions.hpp"
#include "ImageFunctions.hpp"
#include <FreeImage.h>
#include <cstring>

#define FLT FILTER_BILINEAR

int load_rgb(unsigned char **buffer,
	char* filename, unsigned width, unsigned height) {
	unsigned w, h;
	FREE_IMAGE_FORMAT fmt = FreeImage_GetFileType(filename, 0);
	if (fmt == FIF_UNKNOWN) {
		Dprintf(_("Unknown image format, guessing from extension\n"));
		fmt = FreeImage_GetFIFFromFilename(filename);
		if (fmt == FIF_UNKNOWN) {
			Dprintf(_("Unable to guess format from extension\n"));
			return -ENOTSUP;
		}
	}
	if (!FreeImage_FIFSupportsReading(fmt)) {
		Dprintf(_("Unsupported image format\n"));
		return -ENOTSUP;
	}

	FIBITMAP *tmp = FreeImage_Load(fmt, filename);
	if (!tmp) {
		Dprintf(_("Error loading image\n"));
		return 0;
	}
	w = FreeImage_GetWidth(tmp);
	h = FreeImage_GetHeight(tmp);
	FIBITMAP* img = FreeImage_ConvertTo24Bits(tmp);
	FreeImage_Unload(tmp);

	if (w != width || h != height) {
		tmp = img;
		w = width;
		h = height;
		img = FreeImage_Rescale(tmp, width, height, FLT);
		FreeImage_Unload(tmp);
	}
	*buffer = new unsigned char[width * height * 3];
	memcpy(*buffer, FreeImage_GetBits(img), width * height * 3);
	FreeImage_Unload(img);
	return 0;
}

int save_rgb(unsigned char* buffer, unsigned width, unsigned height,
	char* filename) {
	FIBITMAP *bmp = FreeImage_Allocate(width, height, 24);
	if (!bmp) {
		Dprintf(_("Unable to allocate the bitmap for saving\n"));
		return -ENOMEM;
	}
	memcpy(FreeImage_GetBits(bmp), buffer, width * height * 3);
	
	FreeImage_Save(FIF_PNG, bmp, filename, 0);
	FreeImage_Unload(bmp);
	return 0;
}
