#include "PSO.h"
#include <ctime>
#include <cstdlib> /*for rand()*/
#include <iostream>

/*����[min, max]���H�����*/
double RandDouble(double min, double max)
{
	return min + (max - min) * ((double)rand() / RAND_MAX);
}

void algo_PSO::RunALG(int _dim, int _pop_size)
{
	srand(time(NULL));
	dim = _dim;
	pop_size = _pop_size;
	mnfes = dim * 10000;
	nfes = 0;
	
	/*Initialization*/
	Init(); /*generate initial solution in population*/

	while (nfes < mnfes)
	{
		Transition();
		Evaluation();
		Determination();
	}

	cout << "Best fitness : " << g_best_fit << endl;
	cout << "Best solution : ";
	for (double x : g_best)
		cout << x << " ";
	cout << endl;
}

/*�ͦ�dim * pop_size�ӡA��s��l position & velocity*/
void algo_PSO::Init()
{
	p_recent_fit.resize(pop_size);

	for (int i = 0; i < pop_size; ++i)
	{
		vector<double> ind_p(dim);
		vector<double> ind_v(dim);

		for (int j = 0; j < dim; ++j)
		{
			ind_p[j] = RandDouble(-32.0, 32.0); /*����-32~32���H����� for individual position*/
			ind_v[j] = RandDouble(-1.0, 1.0);   /*����-1~1���H����� for individual velocity*/
		}
		population.push_back(ind_p);
		velocity.push_back(ind_v);
		p_recent_fit[i] = Ackley().AckleyProblem(population[i]); /*�p���l��fitness*/
	}
	p_best = population;           /*��l��p_best*/
	p_best_fit = p_recent_fit;     /*��l��p_best_fit*/

	auto best_it = min_element(p_recent_fit.begin(), p_recent_fit.end());
	int best_idx = distance(p_recent_fit.begin(), best_it);
	g_best = population[best_idx]; /*��l��g_best*/
	g_best_fit = *best_it;         /*��l��g_best_fit*/
}

/*�ͦ�dim * pop_size�ӡA��s�s�� position & velocity*/
void algo_PSO::Transition()
{
	for (int i = 0; i < pop_size; ++i)
	{
		double w = 0.5; /*�D���v��*/
		double a1 = 2;  /*�����v��*/
		double a2 = 2;  /*�s���v��*/

		for (int j = 0; j < dim; ++j)
		{
			double r1 = RandDouble(0.0, 1.0); /*�����H�����˾��v*/
			double r2 = RandDouble(0.0, 1.0); /*�s���H�����˾��v*/

			velocity[i][j] = w * velocity[i][j] 
				             + a1 * r1 * (p_best[i][j] - population[i][j]) 
				             + a2 * r2 * (g_best[j] - population[i][j]);
			population[i][j] = population[i][j] + velocity[i][j];
		}
	}
}

/*�p���e��(population)��fitness��Jp_recent_fit*/
void algo_PSO::Evaluation()
{
	for (int i = 0; i < pop_size; ++i)
	{
		p_recent_fit[i] = Ackley().AckleyProblem(population[i]);
		++nfes;
	}
}

/*update p_best & g_best*/
void algo_PSO::Determination()
{
	for (int i = 0; i < pop_size; ++i)
	{
		if (p_recent_fit[i] < p_best_fit[i])
		{
			p_best_fit[i] = p_recent_fit[i];
			p_best[i] = population[i];

			if (p_recent_fit[i] < g_best_fit)
			{
				g_best_fit = p_recent_fit[i];
				g_best = population[i];
			}
		}
	}
}