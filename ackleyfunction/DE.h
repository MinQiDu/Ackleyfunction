#ifndef DE_H
#define DE_H
#include "Ackley.h"
#include <random>    /* for random device & mt19937 */
using namespace std;

class algo_DE
{
public:
	algo_DE();         /* constructor �ŧi */
	void RunALG(const int& dim, const int& pop_size, const double& cross_rate, const double& donor_rate);
private:
	random_device rd;  /* �ŧi�üƤ��� */
	mt19937 gen;

	int dim;
	int pop_size;
	int mnfes;
	int nfes;
	double CR; /* cross_rate */
	double F;  /* =F, donor_rate */

	vector<vector<double>> pop;         /* dim*pop_size �Ӹs��� */
	vector<vector<double>> donor_pop;   /* �ܲ��᪺ donor �� */
	vector<vector<double>> current_pop; /* crossover �᪺�s�� */
	vector<double> fit;                 /* ��Ѫ� fitness */
	vector<double> current_fit;         /* �s�Ѫ� fitness */
	double best_fit;                    /* �����̨�fitness */
	vector<double> best_sol;            /* �����̨θ� */

	vector<double> fit_record;          /* ��X���ĵ��G�� */
	int r_count = 0;

	void Init();          /* ���ͪ�ldim*pop_size�Ӹs��� */
	void Mutation();      /* ����donor�� */
	void Crossover();     /* ��� & donor�� ��e�ͦ�recent�� */
	void Evaluation();    /* �p��recent�Ѫ� fitness */
	void Determination(); /* update��(��� or recent��)�i�����U�@�N��
						  �Aupdate best_fit */
};

#endif
