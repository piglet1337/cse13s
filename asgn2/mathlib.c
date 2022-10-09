#include "mathlib.h"
#include <math.h>

//Define EPSILON constant.
#define EPSILON 1e-10

//my_sin function: takes double input and computes sin of input.
double my_sin(double x) {
	double total = x;
	double term = x;
	for (int n=1; term>EPSILON; n+=1) {
		term = (x/(2*n)) * (x/(2*n+1)) * term;
		total += (n%2==0) ? term : -term;
	}
	return total;
}

//my_cos function: takes double input and computes cos of input.
double my_cos(double x) {
	double total = 1.0;
	double term = 1.0;
	for (int n=1; term>EPSILON; n+=1) {
		term = (x/(2*n)) * (x/(2*n-1)) * term;
		total += (n%2==0) ? term : -term;
	}
	return total;
}

//my_arcsin function: takes double input and computes arcsin of input.
double my_arcsin(double x) {
	double curr = 0;
	double prev = 1.5;
	int neg = 0;
	if (x<0.0) {
		x*=-1.0;
		neg = 1;	
	}
	while (curr-prev >= EPSILON || curr-prev <= -EPSILON) {
		prev = curr;
		curr  = curr - (my_sin(curr)-x)/(my_cos(curr));
	}
	return (neg) ? -curr : curr;
}

//my_arccos function: takes double input and computes arccos of input.
double my_arccos(double x) {
	return M_PI/2-my_arcsin(x);
}

//provided square_root function.
double square_root(double x) {
	double m, l = 0.0;
	double h = (x < 1) ? 1 : x;
	double range;
	do {
		m = (l + h) / 2.0;
		if (m * m < x) {
			l = m;
		} else {
			h = m;
		}
		range = (l > h) ? l - h : h - l;
	} while (range > EPSILON);
	return m;
}

//my_arctan function: takes double input and computes arctan of input.
double my_arctan(double x) {
	return my_arcsin(x/square_root(x*x+1));
}

//provided Exp function.
static double Exp(double x) {
	double t = 1.0;
	double y = 1.0;
	for (double k = 1.0; t > EPSILON; k += 1.0) {
		t *= x / k;
		y += t;
	}
	return y;
}

//my_log function: takes double input and computes log of input.
double my_log(double x) {
	double curr = 1.0;
	double prev = 0.0;
	while (curr-prev<=-EPSILON || curr-prev>=EPSILON) {
		prev = curr;
		double expon = Exp(curr);
		curr = curr + (x-expon)/expon;
	}
	return curr;
