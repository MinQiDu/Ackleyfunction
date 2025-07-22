#include "DE.h"
#include <ctime>
#include <cstdlib>
#include <algorithm> /* for auto */
#include <iostream>
#include <fstream>
#include <string>

/* Constructor 初始化亂數引擎 */
algo_DE::algo_DE() 
	: gen(rd()) /* 用 rd 初始化 gen（記住這只能放在初始化列表中） */
{
}

/* 執行演算法 */
void algo_DE::RunALG(const int& _dim, const int& _pop_size, const double& _CR, const double& _F)
{
	dim = _dim;
	pop_size = _pop_size;
	mnfes = dim * 10000;
	nfes = 0;
	CR = _CR;
	F = _F;
	
	/* Initialization */
	Init();

	fit_record.resize(mnfes); /* 輸出收斂結果用 */
	r_count = 0;

	while (nfes < mnfes)
	{
		Mutation();
		Crossover();
		Evaluation();
		Determination();
	}

	/* 最終輸出最佳fitness & 最佳解 */
	cout << "Best fitness : " << best_fit << endl;
	cout << "Best solution : ";
	for (int i = 0; i < dim; ++i)
		cout << best_sol[i] << " ";
	cout << endl;	

	/* 創建文字檔保存best_fit紀錄 */
	ofstream file("fitness_DE_dim" + to_string(dim) + "_pop" + to_string(pop_size) + "_CR" + to_string(int(CR*10)) + "_F" + to_string(int(F*10)) + ".txt");
	for (int i = 0; i < mnfes; ++i)
	{
		file << i + 1 << " " << fit_record[i] << "\n";
	}
	file.close();

	/* 創建.plt檔生成圖片 */
	ofstream plot("plot_DE.plt");
	plot << "set terminal png size 800, 600\n";
	plot << "set output 'result_Ackley_DE_dim" << dim << "_pop" << pop_size << "_CR" << CR*10 << "_F" << F*10 << ".png'\n";
	plot << "set title 'Convergence with DE on AckleyFunction'\n";
	plot << "set xlabel 'Evaluation times'\n";
	plot << "set ylabel 'Fitness'\n";
	plot << "set xrange[0:" << mnfes << "]\n";
	plot << "set yrange[0:30]\n";
	plot << "plot 'fitness_DE_dim" << dim << "_pop" << pop_size << "_CR" << CR*10 << "_F" << F*10 << ".txt' using 1:2 with lines title 'with DE'\n";
	plot.close();
}

/* 產生初始解 */
void algo_DE::Init()
{
	uniform_real_distribution<double> dist_init(-32.0, 32.0); /* -32.0 ~32.0 實數均勻分布 for 生成初始解 */

	pop.clear();
	donor_pop.clear();
	current_pop.clear();
	fit.clear();
	current_fit.clear();

	fit.resize(pop_size);
	current_fit.resize(pop_size);

	for (int i = 0; i < pop_size; ++i)
	{
		vector<double> ind(dim);

		for (int j = 0; j < dim; ++j)
		{
			ind[j] = dist_init(gen);
		}
		pop.push_back(ind);                       /* 初始化原解 */
		donor_pop.push_back(ind);                 /* 初始化donor解 */
		current_pop.push_back(ind);               /* 初始化recent解 */
		fit[i] = Ackley().AckleyProblem(pop[i]);  /* 初始化原解fit */
		current_fit[i] = fit[i];                  /* 初始化recent解fitness */
	}
	auto best_it = min_element(fit.begin(), fit.end());
	int best_idx = distance(fit.begin(), best_it);
	best_fit = *best_it;                          /* 初始化best_fit */
	best_sol = pop[best_idx];                     /* 初始化best_sol */
}

/* 產生donor解 */
void algo_DE::Mutation()
{
	uniform_int_distribution<int> dist_idx(0, pop_size - 1);

	for (int i = 0; i < pop_size; ++i)
	{
		int r1, r2, r3;

		do { r1 = dist_idx(gen); } while (r1 == i);
		do { r2 = dist_idx(gen); } while (r2 == i || r2 == r1);
		do { r3 = dist_idx(gen); } while (r3 == i || r3 == r2 || r3 == r1);

		for (int j = 0; j < dim; ++j)
		{
			donor_pop[i][j] = pop[r1][j]
				+ F * (pop[r2][j] - pop[r3][j]);
		}
	}

}

/* 原解和donor解交叉產生current解 */
void algo_DE::Crossover()
{
	uniform_int_distribution<int> dist_idx(0, dim - 1);
	uniform_real_distribution<double> dist_real(0.0, 1.0);

	for (int i = 0; i < pop_size; ++i)
	{
		int jrand = dist_idx(gen);     /* 保證必有至少一個idx有變異 */

		for (int j = 0; j < dim; ++j)
		{
			double r = dist_real(gen);
			
			if (r < CR || jrand == j) /* 當隨機[0, 1]浮點數小於cross_rate或是隨機idx等於當前idx */
			{
				current_pop[i][j] = donor_pop[i][j]; /* 就變異為新解 */
			}
			else
			{
				current_pop[i][j] = pop[i][j];       /* 否則維持原解 */
			}
		}
	}
}

/* 計算current解的fitness，計入current_fit */
void algo_DE::Evaluation()
{
	for (int i = 0; i < pop_size; ++i)
	{
		current_fit[i] = Ackley().AckleyProblem(current_pop[i]);
		++nfes;
	}

}

/* 比較新舊fitness,update population & fit，update best_sol & best_fit */
void algo_DE::Determination()
{
	for (int i = 0; i < pop_size; ++i)
	{
		if (current_fit[i] < fit[i])       /* 當current解優於原解，更新 */
		{
			fit[i] = current_fit[i];
			pop[i] = current_pop[i];

			if (current_fit[i] < best_fit) /* 當current解優於最佳解，更新 */
			{
				best_fit = current_fit[i];
				best_sol = current_pop[i];
			}
		}
		fit_record[r_count] = best_fit;
		++r_count;
	}
}