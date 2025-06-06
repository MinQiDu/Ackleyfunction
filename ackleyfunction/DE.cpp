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

	fit_record.resize(mnfes); /*輸出收斂結果用*/
	r_count = 0;

	while (nfes < mnfes)
	{
		Mutation();
		Crossover();
		Evaluation();
		Determination();
	}

	/*最終輸出最佳fitness & 最佳解*/
	cout << "Best fitness : " << best_fit << endl;
	cout << "Best solution : ";
	for (int i = 0; i < dim; ++i)
		cout << best_sol[i] << " ";
	cout << endl;	

	/*創建文字檔保存best_fit紀錄*/
	ofstream file("fitness_DE_dim" + to_string(dim) + "_pop" + to_string(pop_size) + ".txt");
	for (int i = 0; i < mnfes; ++i)
	{
		file << i + 1 << " " << fit_record[i] << "\n";
	}
	file.close();

	/*創建.plt檔生成圖片*/
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

/*產生初始解*/
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
		population.push_back(ind);                      /*初始化原解*/
		donor_population.push_back(ind);                /*初始化donor解*/
		recent_population.push_back(ind);               /*初始化recent解*/
		fit[i] = Ackley().AckleyProblem(population[i]); /*初始化原解fit*/
		recent_fit[i] = fit[i];                         /*初始化recent解fitness*/
	}
	auto best_it = min_element(fit.begin(), fit.end());
	int best_idx = distance(fit.begin(), best_it);
	best_fit = *best_it;                                /*初始化best_fit*/
	best_sol = population[best_idx];                    /*初始化best_sol*/
}

/*產生donor解*/
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

/*原解和donor解交叉產生recent解*/
void algo_DE::Crossover()
{
	for (int i = 0; i < pop_size; ++i)
	{
		int jrand = rand() % dim;     /*保證必有至少一個idx有變異*/

		for (int j = 0; j < dim; ++j)
		{
			double r = (double)rand() / RAND_MAX;
			
			if (r < cr || jrand == j) /*當隨機[0, 1]浮點數小於cross_rate或是隨機idx等於當前idx*/
			{
				recent_population[i][j] = donor_population[i][j]; /*就變異為新解*/
			}
			else
			{
				recent_population[i][j] = population[i][j];       /*否則維持原解*/
			}
		}
	}
}

/*計算recent解的fitness，計入recent_fit*/
void algo_DE::Evaluation()
{
	for (int i = 0; i < pop_size; ++i)
	{
		recent_fit[i] = Ackley().AckleyProblem(recent_population[i]);
		++nfes;
	}

}

/*比較新舊fitness,update population & fit，update best_sol & best_fit*/
void algo_DE::Determination()
{
	for (int i = 0; i < pop_size; ++i)
	{
		if (recent_fit[i] < fit[i])       /*當recent解優於原解，更新*/
		{
			fit[i] = recent_fit[i];
			population[i] = recent_population[i];

			if (recent_fit[i] < best_fit) /*當recent解優於最佳解，更新*/
			{
				best_fit = recent_fit[i];
				best_sol = recent_population[i];
			}
		}
		fit_record[r_count] = best_fit;
		++r_count;
	}
}