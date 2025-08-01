#include "JADE.h"
#include <ctime>
#include <cstdlib>
#include <algorithm> /* for auto */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>     /* for sqrt */

/* Constructor ��l�ƶüƤ��� */
algo_JADE::algo_JADE() 
	: gen(rd()) /* �� rd ��l�� gen�]�O��o�u���b��l�ƦC���^ */
{ 
}

/* ����t��k */
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

	fit_record.resize(mnfes); /* ��X���ĵ��G�� */
	r_count = 0;

	while (nfes < mnfes)
	{
		Mutation();
		Crossover();
		Evaluation();
		Determination();
		ParaAdaptation();
	}

	/* �̲׿�X�̨�fitness & �̨θ� */
	cout << "Best fitness : " << best_fit << endl;
	cout << "Best solution : ";
	for (int i = 0; i < dim; ++i)
		cout << best_sol[i] << " ";
	cout << endl;	

	/* �Ыؤ�r�ɫO�sbest_fit���� */
	ofstream file("fitness_JADE_dim" + to_string(dim) + "_pop" + to_string(pop_size) + "_c" + to_string(int(c*100)) + "_p" + to_string(int(p*100)) + ".txt");
	for (int i = 0; i < mnfes; ++i)
	{
		file << i + 1 << " " << fit_record[i] << "\n";
	}
	file.close();

	/* �Ы�.plt�ɥͦ��Ϥ� */
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

/* ���ͪ�l�� */
void algo_JADE::Init()
{
	uniform_real_distribution<double> dist_init(-32.0, 32.0); /* -32.0 ~32.0 ��Ƨ��ä��� for �ͦ���l�� */

	pop.clear();
	donor_pop.clear();
	current_pop.clear();
	fit.clear();
	current_fit.clear();
	A.clear();   /* A �s�L�h�Q�^�O������A��l���� */

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
		pop.emplace_back(ind);/* ��l�ƭ�� */
		donor_pop.emplace_back(ind);/* ��l��donor�� */
		current_pop.emplace_back(ind);/* ��l��current�� */

		double fitness = ackley.AckleyProblem(ind);/* ��l�ƭ��fit */
		fit[i] = fitness;
		current_fit[i] = fitness;/* ��l��current��fitness */
	}

	auto best_it = min_element(fit.begin(), fit.end());
	int best_idx = distance(fit.begin(), best_it);
	best_fit = *best_it;/* ��l��best_fit */
	best_sol = pop[best_idx];/* ��l��best_sol */
}

/*����donor��*/
void algo_JADE::Mutation()
{
	normal_distribution<double> dist_CR(mCR, 0.1); /* �`�A���� for �ͦ����骺 CR */
	cauchy_distribution<double> dist_F(mF, 0.1);   /* �_����� for �ͦ����骺 F */

	for (int i = 0; i < pop_size; ++i)
	{
		/* �ͦ����N�Ҧ����骺 CR[i] & F[i] */
		CR[i] = dist_CR(gen);
		if (CR[i] < 0.0) CR[i] = 0.0;              /* truncate */
		else if (CR[i] > 1.0) CR[i] = 1.0;

		do {
			F[i] = dist_F(gen);
		} while (F[i] <= 0.0 || F[i] > 1.0);       /* truncate */
		
		/* ��Xx_pbest, x_r1, x_r2 �Ӳ��� donor_pop */
		/* x_pbest */
		int p_num = p * pop_size;
		if (p_num < 2) p_num = 2;                                   /* �T�O�ܤ֦�2�ӭԿ���� */
		vector<pair<double, int>> fit_idx(pop_size);                /* pair = { fitness, index } */
		for (int i = 0; i < pop_size; ++i)                          /* �إ� fit_idx �H�ѱƧǦC�X���� fitness �ƦW */
		{
			fit_idx[i] = { fit[i], i };
		}
		sort(fit_idx.begin(), fit_idx.end());                       /* fitness �Ѥp->�j */

		int idx_pbest;
		do {
			uniform_int_distribution<int> dist_pbest(0, p_num - 1); /* ��Ƨ��ä��� for �ͦ�index */
			idx_pbest = fit_idx[dist_pbest(gen)].second;            /* pbest �b pop ���� index */
		} while (idx_pbest == i);                                   /* �T�Oidx_pbest���e����i���P */
		vector<double> x_pbest = pop[idx_pbest];                    /* �o�� x_pbest */

		/* x_r1 */
		uniform_int_distribution<int> dist_r1(0, pop_size - 1);
		int idx_r1;
		do {
			idx_r1 = dist_r1(gen);
		} while (idx_r1 == i || idx_r1 == idx_pbest);
		vector<double> x_r1 = pop[idx_r1];                          /* �o�� x_r1 */

		/* x_r2 */
		uniform_int_distribution<int> dist_r2(0, pop_size + A.size() - 1);
		vector<vector<double>> pop_A = pop;                         /* ���ƻspop */
		pop_A.insert(pop_A.end(), A.begin(), A.end());              /* �A���WA */
		int idx_r2;
		do {
			idx_r2 = dist_r2(gen);
		} while (idx_r2 == i || idx_r2 == idx_pbest || idx_r2 == idx_r1);
		vector<double> x_r2 = pop_A[idx_r2];                        /* �o�� x_r2 */

		for (int j = 0; j < dim; ++j)                               /* ��X�����骺 donor �� */
		{
			donor_pop[i][j] = pop[i][j] + F[i] * (x_pbest[j] - pop[i][j]) + F[i] * (x_r1[j] - x_r2[j]);
		}
	}
}

/* ��ѩMdonor�ѥ�e����current�� */
void algo_JADE::Crossover()
{
	uniform_int_distribution<int> dist_int(0, dim - 1);    /* ��Ƨ��ä��� for �ͦ�index */
	uniform_real_distribution<double> dist_real(0.0, 1.0); /* ��Ƨ��ä��� for �ͦ� r */

	for (int i = 0; i < pop_size; ++i)
	{
		int jrand = dist_int(gen);       /* �O�ҥ����ܤ֤@��idx���ܲ� */

		for (int j = 0; j < dim; ++j)
		{
			double r = dist_real(gen);
			
			if (r < CR[i] || jrand == j) /* ���H��[0, 1]�B�I�Ƥp�� cross_rate �άO�H�� idx �����e idx */
			{
				current_pop[i][j] = donor_pop[i][j]; /* �N�ܲ����s�� */
			}
			else
			{
				current_pop[i][j] = pop[i][j];       /* �_�h������� */
			}
		}
	}
}

/* �p��current�Ѫ�fitness�A�p�Jcurrent_fit */
void algo_JADE::Evaluation()
{
	for (int i = 0; i < pop_size; ++i)
	{
		current_fit[i] = Ackley().AckleyProblem(current_pop[i]);
		++nfes;
	}

}

/* ����s��fitness,update pop & fit�Aupdate best_sol & best_fit */
void algo_JADE::Determination()
{
	for (int i = 0; i < pop_size; ++i)
	{
		if (current_fit[i] < fit[i])       /* ��current���u���ѡA��s */
		{
			A.push_back(pop[i]);           /* �^�O����Ѧs�J A */
			fit[i] = current_fit[i];
			pop[i] = current_pop[i];
			sF.push_back(F[i]);            /* ���\��s���骺 F �s�J sF */
			sCR.push_back(CR[i]);          /* ���\��s���骺 CR �s�J sCR */

			if (current_fit[i] < best_fit) /* ��current���u��̨θѡA��s */
			{
				best_fit = current_fit[i];
				best_sol = current_pop[i];
			}
		}
		fit_record[r_count] = best_fit;
		++r_count;
	}
}

/* (1)maintain A�A (2)mCR & mF adaptation */
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
	mCR = (1 - c) * mCR + c * mean_sCR;     /* �o��smCR */

	/* mF */
	double sum_sF2 = 0.0;
	double sum_sF = 0.0;
	for (double f : sF)
	{
		sum_sF2 += f * f;
		sum_sF += f;
	}
	double mean_sF = sum_sF2 / sum_sF;     /* Lehmer mean */
	mF = (1 - c) * mF + c * mean_sF;       /* �o��smF */

	sCR.clear();
	sF.clear();
}