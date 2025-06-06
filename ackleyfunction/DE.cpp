#include "DE.h"
#include <ctime>
#include <cstdlib>
#include <algorithm> /*for auto*/
#include <iostream>
#include <fstream>
#include <string>

double RandDouble(const double& min, const double& max)
{
	return min + ((max - min) * ((double)rand() / RAND_MAX));
}

void algo_DE::RunALG(const int& _dim, const int& _pop_size, const double& _cross_rate, const double& _donor_rate)
{
	srand(time(NULL));
	dim = _dim;
	pop_size = _pop_size;
	mnfes = dim * 10000;
	nfes = 0;
	cr = _cross_rate;
	dr = _donor_rate;
	
	/*Initialization*/
	Init();

	fit_record.resize(mnfes); /*��X���ĵ��G��*/
	r_count = 0;

	while (nfes < mnfes)
	{
		Mutation();
		Crossover();
		Evaluation();
		Determination();
	}

	/*�̲׿�X�̨�fitness & �̨θ�*/
	cout << "Best fitness : " << best_fit << endl;
	cout << "Best solution : ";
	for (int i = 0; i < dim; ++i)
		cout << best_sol[i] << " ";
	cout << endl;	

	/*�Ыؤ�r�ɫO�sbest_fit����*/
	ofstream file("fitness_DE_dim" + to_string(dim) + "_pop" + to_string(pop_size) + ".txt");
	for (int i = 0; i < mnfes; ++i)
	{
		file << i + 1 << " " << fit_record[i] << "\n";
	}
	file.close();

	/*�Ы�.plt�ɥͦ��Ϥ�*/
	ofstream plot("plot_DE.plt");
	plot << "set terminal png size 800, 600\n";
	plot << "set output 'result_Ackley_DE_dim" << dim << "_pop" << pop_size << ".png'\n";
	plot << "set title 'Convergence with DE on AckleyFunction'\n";
	plot << "set xlabel 'Evaluation times'\n";
	plot << "set ylabel 'Fitness'\n";
	plot << "set xrange[0:" << mnfes << "]\n";
	plot << "set yrange[0:30]\n";
	plot << "plot 'fitness_DE_dim" << dim << "_pop" << pop_size << ".txt' using 1:2 with lines title 'with DE'\n";
	plot.close();
}

/*���ͪ�l��*/
void algo_DE::Init()
{
	population.clear();
	donor_population.clear();
	recent_population.clear();
	fit.clear();
	recent_fit.clear();

	fit.resize(pop_size);
	recent_fit.resize(pop_size);

	for (int i = 0; i < pop_size; ++i)
	{
		vector<double> ind(dim);

		for (int j = 0; j < dim; ++j)
		{
			ind[j] = RandDouble(-32.0, 32.0);
		}
		population.push_back(ind);                      /*��l�ƭ��*/
		donor_population.push_back(ind);                /*��l��donor��*/
		recent_population.push_back(ind);               /*��l��recent��*/
		fit[i] = Ackley().AckleyProblem(population[i]); /*��l�ƭ��fit*/
		recent_fit[i] = fit[i];                         /*��l��recent��fitness*/
	}
	auto best_it = min_element(fit.begin(), fit.end());
	int best_idx = distance(fit.begin(), best_it);
	best_fit = *best_it;                                /*��l��best_fit*/
	best_sol = population[best_idx];                    /*��l��best_sol*/
}

/*����donor��*/
void algo_DE::Mutation()
{
	for (int i = 0; i < pop_size; ++i)
	{
		int r1, r2, r3;

		do { r1 = rand() % pop_size; } while (r1 == i);
		do { r2 = rand() % pop_size; } while (r2 == i || r2 == r1);
		do { r3 = rand() % pop_size; } while (r3 == i || r3 == r2 || r3 == r1);

		for (int j = 0; j < dim; ++j)
		{
			donor_population[i][j] = population[r1][j]
				+ dr* (population[r2][j] - population[r3][j]);
		}
	}

}

/*��ѩMdonor�ѥ�e����recent��*/
void algo_DE::Crossover()
{
	for (int i = 0; i < pop_size; ++i)
	{
		int jrand = rand() % dim;     /*�O�ҥ����ܤ֤@��idx���ܲ�*/

		for (int j = 0; j < dim; ++j)
		{
			double r = (double)rand() / RAND_MAX;
			
			if (r < cr || jrand == j) /*���H��[0, 1]�B�I�Ƥp��cross_rate�άO�H��idx�����eidx*/
			{
				recent_population[i][j] = donor_population[i][j]; /*�N�ܲ����s��*/
			}
			else
			{
				recent_population[i][j] = population[i][j];       /*�_�h�������*/
			}
		}
	}
}

/*�p��recent�Ѫ�fitness�A�p�Jrecent_fit*/
void algo_DE::Evaluation()
{
	for (int i = 0; i < pop_size; ++i)
	{
		recent_fit[i] = Ackley().AckleyProblem(recent_population[i]);
		++nfes;
	}

}

/*����s��fitness,update population & fit�Aupdate best_sol & best_fit*/
void algo_DE::Determination()
{
	for (int i = 0; i < pop_size; ++i)
	{
		if (recent_fit[i] < fit[i])       /*��recent���u���ѡA��s*/
		{
			fit[i] = recent_fit[i];
			population[i] = recent_population[i];

			if (recent_fit[i] < best_fit) /*��recent���u��̨θѡA��s*/
			{
				best_fit = recent_fit[i];
				best_sol = recent_population[i];
			}
		}
		fit_record[r_count] = best_fit;
		++r_count;
	}
}