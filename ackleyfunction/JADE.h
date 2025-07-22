#ifndef JADE_H
#define JADE_H
#include "Ackley.h"

class algo_JADE
{
public:
	algo_JADE();       /* constructor �ŧi */
	void RunALG(const int& dim, const int& pop_size, const double& CR, const double& F, const double& c, const double& p);
private:
	random_device rd;  /*�ŧi�üƤ���*/
	mt19937 gen;

	int dim;
	int pop_size;
	int mnfes;
	int nfes;
	double mCR;        /*adaptive parameter 1 (cross_rate)*/
	double mF;         /*adaptive parameter 2 (= F, donor_rate)*/
	vector<double> CR; /*�s���N�C�ӭ��骺 CR*/
	vector<double> F;  /*�s���N�C�ӭ��骺 F*/
	double c;          /*adaptation rate*/
	double p;          /*top % pop chosen by pbest*/

	vector<double> sCR;                 /*�s���N�����\��s�����骺CR*/
	vector<double> sF;                  /*�s���N�����\��s�����骺F*/
	vector<vector<double>> A;           /*�s���N���Q�^�O������ѡAsize = pop_size*/
	vector<vector<double>> pop;         /*dim*pop_size �Ӹs���*/
	vector<vector<double>> donor_pop;   /*�ܲ��᪺ donor ��*/
	vector<vector<double>> current_pop; /*crossover �᪺�s��*/
	vector<double> fit;                 /*��Ѫ� fitness*/
	vector<double> current_fit;         /*�s�Ѫ� fitness*/
	double best_fit;                    /*�����̨�fitness*/
	vector<double> best_sol;            /*�����̨θ�*/

	vector<double> fit_record;          /*��X���ĵ��G��*/
	int r_count = 0;

	void Init();           /*���ͪ�ldim*pop_size�Ӹs���*/
	void Mutation();       /*����donor��*/
	void Crossover();      /*��� & donor�� ��e�ͦ�recent��*/
	void Evaluation();     /*�p��recent�Ѫ� fitness*/
	void Determination();  /*update��(��� or recent��)�i�����U�@�N��
						   �Aupdate best_fit*/
	void ParaAdaptation(); /*��s mCR & mF*/
};

#endif
