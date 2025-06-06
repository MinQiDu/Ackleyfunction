#include "Ackley.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double Ackley::AckleyProblem(const vector<double> &x)
{
	double a = 20.0;
	double b = 0.2;
	double c = 2 * M_PI;
	int d = x.size();
	double sum1 = 0.0;
	double sum2 = 0.0;
	for (double xi : x)
	{
		sum1 += xi * xi;
		sum2 += cos(c * xi);
	}

	return -a * exp(-b * sqrt((sum1 / d))) - exp(sum2 / d) + a + exp(1.0);
}