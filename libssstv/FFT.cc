#include "FFT.hpp"
#include <math.h>

static const double TWO_PI = acos(-1.0) * 2.0;

void fft(my_complex *arr, int length, int ifft) {
	if (length <= 1)
		return;
	my_complex wn;
	int k, hlen = length >> 1;

	if (!ifft) {
		wn.real = cos(TWO_PI / length);
		wn.imag = sin(TWO_PI / length);
	}
	else {
		wn.real = cos(-TWO_PI / length);
		wn.imag = sin(-TWO_PI / length);
	}
	my_complex w;
	w.real = 1;

	my_complex a0[length >> 1];
	my_complex a1[length >> 1];
	for (k = 0; k < length; k += 2) {
		a0[k >> 1] = arr[k];
		a1[k >> 1] = arr[k + 1];
	}
	fft(a0, hlen, ifft);
	fft(a1, hlen, ifft);

	for (k = 0; k < hlen; k++) {
		arr[k] = my_complex_add(a0[k], my_complex_mult(w, a1[k]));
		arr[k + hlen] = my_complex_sub(a0[k], my_complex_mult(w, a1[k]));
		w = my_complex_mult(w, wn);

		if (ifft) {
			arr[k] = my_complex_scale(arr[k], 0.5);
			arr[k + hlen] = my_complex_scale(arr[k], 0.5);
		}
	}
}


