#include "my_complex.hpp"
#include <cstdlib>
#include <cmath>

int my_complex_cmp(my_complex* a, my_complex* b) {
	double d = a->real - b->real;
	if (d < 0.0001)
		return 0;
	return d > 0 ? 1 : -1;
}

my_complex my_complex_add(my_complex a, my_complex b) {
	my_complex ret;
	ret.real = a.real + b.real;
	ret.imag = a.imag + b.imag;
	return ret;
}

my_complex my_complex_sub(my_complex a, my_complex b) {
	my_complex ret;
	ret.real = a.real - b.real;
	ret.imag = a.imag - b.imag;
	return ret;
}

my_complex my_complex_mult(my_complex a, my_complex b) {
	my_complex ret;
	ret.real = a.real * b.real - a.imag * b.imag;
	ret.imag = a.real * b.imag + a.imag * b.real;
	return ret;
}

my_complex my_complex_scale(my_complex a, double b) {
	my_complex ret;
	ret.real = a.real * b;
	ret.imag = a.imag * b;
	return ret;
}

double my_complex_abs(my_complex a) {
	return sqrt(a.real * a.real + a.imag * a.imag);
}

