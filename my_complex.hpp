#ifndef __COMPLEX_H__
#define __COMPLEX_H__

typedef struct my_complex_t {
	double real;
	double imag;
} my_complex;

int my_complex_cmp(my_complex* a, my_complex* b);
my_complex my_complex_add(my_complex a, my_complex b);
my_complex my_complex_sub(my_complex a, my_complex b);
my_complex my_complex_mult(my_complex a, my_complex b);
my_complex my_complex_scale(my_complex a, double b);
double my_complex_abs(my_complex a);

#endif //__COMPLEX_H__
