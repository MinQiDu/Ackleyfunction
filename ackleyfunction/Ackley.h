#ifndef ACKLEY_H
#define ACKLEY_H
#include <vector>
using namespace std;

class Ackley
{
public:
	double AckleyProblem(const vector<double> &sol, const int &d);
};

#endif