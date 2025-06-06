#include "PSO.h"
#include "DE.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	int dim = atoi(argv[1]);
	int pop_size = atoi(argv[2]);
	double cr = atof(argv[3]);
	double dr = atof(argv[4]);
	string algo_type;

	bool canrun = 0;

	while (canrun == 0)
	{
		cout << "Choose Algorithm ( PSO / DE )" << endl;
		getline(cin, algo_type);

		if (algo_type == "PSO")
		{
			cout << "algo_type = " << algo_type << endl;
			canrun = 1;
			algo_PSO algo;
			algo.RunALG(dim, pop_size);
		}

		else if (algo_type == "DE")
		{
			cout << "algo_type : " << algo_type << endl;
			canrun = 1;
			algo_DE algo;
			algo.RunALG(dim, pop_size, cr, dr);
		}
	}

}