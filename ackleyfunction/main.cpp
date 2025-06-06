#include "PSO.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	int dim = atoi(argv[1]);
	int pop_size = atoi(argv[2]);
	string algo_type;

	bool canrun = 0;

	while (canrun == 0)
	{
		cout << "Choose Algorithm ( PSO )" << endl;
		getline(cin, algo_type);

		if (algo_type == "PSO")
		{
			cout << "algo_type = " << algo_type << endl;
			canrun = 1;
			algo_PSO algo;
			algo.RunALG(dim, pop_size);
		}
	}

}