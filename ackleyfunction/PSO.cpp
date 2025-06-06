#include "PSO.h"
#include <ctime>
#include <cstdlib> /*for rand()*/
#include <iostream>
#include <fstream>
#include <string>

/*產生[min, max]的隨機實數*/
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

	fit_record.resize(mnfes);
	r_count = 0;

	while (nfes < mnfes)
	{
		Transition();
		Evaluation();
		Determination();

		if (nfes % 1000 == 0)
		{
			cout << g_best_fit << endl;
		}
	}

	/*四捨五入輸出*/
	//if (abs(g_best_fit) < 1e-10) g_best_fit = 0.0;
	cout << "Best fitness : " << g_best_fit << endl;
	cout << "Best solution : ";
	for (int j = 0; j < dim; ++j)
	{
		if (abs(g_best[j]) < 1e-10)
		{
			//g_best[j] = 0.0;
		}
		cout << g_best[j] << " ";
	}
	cout << endl;

	/*創建文字檔保存best_fit紀錄*/
	ofstream file("fitness_PSO_dim" + to_string(dim) + "_pop" + to_string(pop_size) + ".txt");
	for (int i = 0; i < mnfes; ++i)
	{
		file << i + 1 << " " << fit_record[i] << "\n";
	}
	file.close();

	/*創建.plt檔生成圖片*/
	ofstream plot("plot_PSO.plt");
	plot << "set terminal png size 800, 600\n";
	plot << "set output 'result_Ackley_PSO_dim" << dim << "_pop" << pop_size << ".png'\n";
	plot << "set title 'Convergence with PSO on AckleyFunction'\n";
	plot << "set xlabel 'Evaluation times'\n";
	plot << "set ylabel 'Fitness'\n";
	plot << "set xrange [0:" << mnfes << "]\n";
	plot << "set yrange [0:30]\n";
	plot << "plot 'fitness_PSO_dim" << dim << "_pop" << pop_size << ".txt' using 1:2 with lines title 'with PSO'\n";
	plot.close();

}

/*生成dim * pop_size個，整群初始 position & velocity*/
void algo_PSO::Init()
{
	p_recent_fit.resize(pop_size);

	for (int i = 0; i < pop_size; ++i)
	{
		vector<double> ind_p(dim);
		vector<double> ind_v(dim);

		for (int j = 0; j < dim; ++j)
		{
			ind_p[j] = RandDouble(-32.0, 32.0); /*產生-32~32的隨機實數 for individual position*/
			ind_v[j] = RandDouble(-1.0, 1.0);   /*產生-1~1的隨機實數 for individual velocity*/
		}
		population.push_back(ind_p);
		velocity.push_back(ind_v);
		p_recent_fit[i] = Ackley().AckleyProblem(population[i]); /*計算初始解fitness*/
	}
	p_best = population;           /*初始化p_best*/
	p_best_fit = p_recent_fit;     /*初始化p_best_fit*/

	auto best_it = min_element(p_recent_fit.begin(), p_recent_fit.end());
	int best_idx = distance(p_recent_fit.begin(), best_it);
	g_best = population[best_idx]; /*初始化g_best*/
	g_best_fit = *best_it;         /*初始化g_best_fit*/
}

/*生成dim * pop_size個，整群新的 position & velocity*/
void algo_PSO::Transition()
{
	for (int i = 0; i < pop_size; ++i)
	{
		double w = 0.5; /*慣性權重*/
		double a1 = 2;  /*個體權重*/
		double a2 = 2;  /*群體權重*/

		for (int j = 0; j < dim; ++j)
		{
			double r1 = RandDouble(0.0, 1.0); /*個體隨機取樣機率*/
			double r2 = RandDouble(0.0, 1.0); /*群體隨機取樣機率*/

			velocity[i][j] = w * velocity[i][j] 
				             + a1 * r1 * (p_best[i][j] - population[i][j]) 
				             + a2 * r2 * (g_best[j] - population[i][j]);
			population[i][j] = population[i][j] + velocity[i][j];
			
			/*clamping防止超出範圍*/
			if (population[i][j] > 32) population[i][j] = 32;
			else if (population[i][j] < -32) population[i][j] = -32;
		}
	}
}

/*計算當前解(population)的fitness放入p_recent_fit*/
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
		fit_record[r_count] = g_best_fit;
		++r_count;
	}
}