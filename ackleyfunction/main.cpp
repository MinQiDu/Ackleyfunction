#include "PSO.h"
#include "DE.h"
#include "JADE.h"
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	int dim = atoi(argv[1]);
	int pop_size = atoi(argv[2]);
	double CR = atof(argv[3]);
	double F = atof(argv[4]);
	string algo_type;

	bool canrun = 0;

	while (canrun == 0)
	{
		cout << "Choose Algorithm ( PSO / DE / JADE)" << endl;
		getline(cin, algo_type);
		transform(algo_type.begin(), algo_type.end(), algo_type.begin(), toupper); /* 輸入大小寫都可以，最後切回大寫 */

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
			algo.RunALG(dim, pop_size, CR, F);
		}
		else if (algo_type == "JADE")
		{
			cout << "algo_type = " << algo_type << endl;
			canrun = 1;

			string _c, _p;
			cout << "Please enter the value of adaptation rate c = ";
			getline(cin, _c);
			cout << "Please enter the value of the top % pop chosen for pbest = ";
			getline(cin, _p);
			double c = stod(_c);
			double p = stod(_p);

			algo_JADE algo;
			algo.RunALG(dim, pop_size, CR, F, c, p);
		}
	}

}