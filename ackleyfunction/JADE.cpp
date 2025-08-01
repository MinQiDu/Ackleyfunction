#include "JADE.h"
#include <ctime>
#include <cstdlib>
#include <algorithm> /* for auto */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>     /* for sqrt */

/* Constructor 初始化亂數引擎 */
algo_JADE::algo_JADE() 
	: gen(rd()) /* 用 rd 初始化 gen（記住這只能放在初始化列表中） */
{ 
}

/* 執行演算法 */
void algo_JADE::RunALG(const int& _dim, const int& _pop_size, const double& _CR, const double& _F, const double& _c, const double& _p)
{
	dim = _dim;
	pop_size = _pop_size;
	mnfes = dim * 2000;
	nfes = 0;
	mCR = _CR;
	mF = _F;
	c = _c;
	p = _p;
	
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
		ParaAdaptation();
	}

	/* 最終輸出最佳fitness & 最佳解 */
	cout << "Best fitness : " << best_fit << endl;
	cout << "Best solution : ";
	for (int i = 0; i < dim; ++i)
		cout << best_sol[i] << " ";
	cout << endl;	

	/* 創建文字檔保存best_fit紀錄 */
	ofstream file("fitness_JADE_dim" + to_string(dim) + "_pop" + to_string(pop_size) + "_c" + to_string(int(c*100)) + "_p" + to_string(int(p*100)) + ".txt");
	for (int i = 0; i < mnfes; ++i)
	{
		file << i + 1 << " " << fit_record[i] << "\n";
	}
	file.close();

	/* 創建.plt檔生成圖片 */
	ofstream plot("plot_JADE.plt");
	plot << "set terminal png size 800, 600\n";
	plot << "set output 'result_Ackley_JADE_dim" << dim << "_pop" << pop_size << "_c" << c*100 << "_p" << p*100 << ".png'\n";
	plot << "set title 'Convergence with JADE on AckleyFunction'\n";
	plot << "set xlabel 'Evaluation times'\n";
	plot << "set ylabel 'Fitness'\n";
	plot << "set xrange[0:" << dim * 2000 << "]\n";
	plot << "set yrange[0:30]\n";
	plot << "plot 'fitness_JADE_dim" << dim << "_pop" << pop_size << "_c" << c*100 << "_p" << p*100 << ".txt' using 1:2 with lines title 'with JADE'\n";
	plot.close();
}

/* 產生初始解 */
void algo_JADE::Init()
{
	uniform_real_distribution<double> dist_init(-32.0, 32.0); /* -32.0 ~32.0 實數均勻分布 for 生成初始解 */

	pop.clear();
	donor_pop.clear();
	current_pop.clear();
	fit.clear();
	current_fit.clear();
	A.clear();   /* A 存過去被淘汰的個體，初始為空 */

	CR.resize(pop_size);
	F.resize(pop_size);
	fit.resize(pop_size);
	current_fit.resize(pop_size);

	pop.reserve(pop_size);
	donor_pop.reserve(pop_size);
	current_pop.reserve(pop_size);

	Ackley ackley; // Construct once

	for (int i = 0; i < pop_size; ++i)
	{
		vector<double> ind(dim);
		for (int j = 0; j < dim; ++j)
		{
			ind[j] = dist_init(gen);
		}
		pop.emplace_back(ind);/* 初始化原解 */
		donor_pop.emplace_back(ind);/* 初始化donor解 */
		current_pop.emplace_back(ind);/* 初始化current解 */

		double fitness = ackley.AckleyProblem(ind);/* 初始化原解fit */
		fit[i] = fitness;
		current_fit[i] = fitness;/* 初始化current解fitness */
	}

	auto best_it = min_element(fit.begin(), fit.end());
	int best_idx = distance(fit.begin(), best_it);
	best_fit = *best_it;/* 初始化best_fit */
	best_sol = pop[best_idx];/* 初始化best_sol */
}

/*產生donor解*/
void algo_JADE::Mutation()
{
	normal_distribution<double> dist_CR(mCR, 0.1); /* 常態分布 for 生成個體的 CR */
	cauchy_distribution<double> dist_F(mF, 0.1);   /* 柯西分布 for 生成個體的 F */

	for (int i = 0; i < pop_size; ++i)
	{
		/* 生成此代所有個體的 CR[i] & F[i] */
		CR[i] = dist_CR(gen);
		if (CR[i] < 0.0) CR[i] = 0.0;              /* truncate */
		else if (CR[i] > 1.0) CR[i] = 1.0;

		do {
			F[i] = dist_F(gen);
		} while (F[i] <= 0.0 || F[i] > 1.0);       /* truncate */
		
		/* 選出x_pbest, x_r1, x_r2 來產生 donor_pop */
		/* x_pbest */
		int p_num = p * pop_size;
		if (p_num < 2) p_num = 2;                                   /* 確保至少有2個候選個體 */
		vector<pair<double, int>> fit_idx(pop_size);                /* pair = { fitness, index } */
		for (int i = 0; i < pop_size; ++i)                          /* 建立 fit_idx 以供排序列出個體 fitness 排名 */
		{
			fit_idx[i] = { fit[i], i };
		}
		sort(fit_idx.begin(), fit_idx.end());                       /* fitness 由小->大 */

		int idx_pbest;
		do {
			uniform_int_distribution<int> dist_pbest(0, p_num - 1); /* 整數均勻分布 for 生成index */
			idx_pbest = fit_idx[dist_pbest(gen)].second;            /* pbest 在 pop 中的 index */
		} while (idx_pbest == i);                                   /* 確保idx_pbest跟當前個體i不同 */
		vector<double> x_pbest = pop[idx_pbest];                    /* 得到 x_pbest */

		/* x_r1 */
		uniform_int_distribution<int> dist_r1(0, pop_size - 1);
		int idx_r1;
		do {
			idx_r1 = dist_r1(gen);
		} while (idx_r1 == i || idx_r1 == idx_pbest);
		vector<double> x_r1 = pop[idx_r1];                          /* 得到 x_r1 */

		/* x_r2 */
		uniform_int_distribution<int> dist_r2(0, pop_size + A.size() - 1);
		vector<vector<double>> pop_A = pop;                         /* 先複製pop */
		pop_A.insert(pop_A.end(), A.begin(), A.end());              /* 再接上A */
		int idx_r2;
		do {
			idx_r2 = dist_r2(gen);
		} while (idx_r2 == i || idx_r2 == idx_pbest || idx_r2 == idx_r1);
		vector<double> x_r2 = pop_A[idx_r2];                        /* 得到 x_r2 */

		for (int j = 0; j < dim; ++j)                               /* 算出此個體的 donor 解 */
		{
			donor_pop[i][j] = pop[i][j] + F[i] * (x_pbest[j] - pop[i][j]) + F[i] * (x_r1[j] - x_r2[j]);
		}
	}
}

/* 原解和donor解交叉產生current解 */
void algo_JADE::Crossover()
{
	uniform_int_distribution<int> dist_int(0, dim - 1);    /* 整數均勻分布 for 生成index */
	uniform_real_distribution<double> dist_real(0.0, 1.0); /* 實數均勻分布 for 生成 r */

	for (int i = 0; i < pop_size; ++i)
	{
		int jrand = dist_int(gen);       /* 保證必有至少一個idx有變異 */

		for (int j = 0; j < dim; ++j)
		{
			double r = dist_real(gen);
			
			if (r < CR[i] || jrand == j) /* 當隨機[0, 1]浮點數小於 cross_rate 或是隨機 idx 等於當前 idx */
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
void algo_JADE::Evaluation()
{
	for (int i = 0; i < pop_size; ++i)
	{
		current_fit[i] = Ackley().AckleyProblem(current_pop[i]);
		++nfes;
	}

}

/* 比較新舊fitness,update pop & fit，update best_sol & best_fit */
void algo_JADE::Determination()
{
	for (int i = 0; i < pop_size; ++i)
	{
		if (current_fit[i] < fit[i])       /* 當current解優於原解，更新 */
		{
			A.push_back(pop[i]);           /* 淘汰的原解存入 A */
			fit[i] = current_fit[i];
			pop[i] = current_pop[i];
			sF.push_back(F[i]);            /* 成功更新個體的 F 存入 sF */
			sCR.push_back(CR[i]);          /* 成功更新個體的 CR 存入 sCR */

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

/* (1)maintain A， (2)mCR & mF adaptation */
void algo_JADE::ParaAdaptation()
{
	/* maintain size of A <= pop_size */
	while (A.size() > pop_size)
	{
		uniform_int_distribution<int> dist_idx(0, A.size() - 1);
		int idx = dist_idx(gen);
		A.erase(A.begin() + idx);
	}

	/* mCR & mF adaptation */
	/* mCR */
	double sum_sCR = 0.0;
	for (double cr : sCR) sum_sCR += cr;
	double mean_sCR = sum_sCR / sCR.size(); /* arithmetic mean */
	mCR = (1 - c) * mCR + c * mean_sCR;     /* 得到新mCR */

	/* mF */
	double sum_sF2 = 0.0;
	double sum_sF = 0.0;
	for (double f : sF)
	{
		sum_sF2 += f * f;
		sum_sF += f;
	}
	double mean_sF = sum_sF2 / sum_sF;     /* Lehmer mean */
	mF = (1 - c) * mF + c * mean_sF;       /* 得到新mF */

	sCR.clear();
	sF.clear();
}