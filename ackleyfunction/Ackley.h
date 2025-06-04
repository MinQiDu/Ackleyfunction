#ifndef ACKLEY_H
#define ACKLEY_H
#include <vector>
using namespace std;

class Ackley
{
public:
	/*calculate the value of sol in ackley function*/
	double AckleyProblem(const vector<double> &x);
};

#endif