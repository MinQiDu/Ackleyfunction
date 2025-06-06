#ifndef DE_H
#define DE_H
#include "Ackley.h"

class algo_DE
{
public:
	void RunALG(const int& dim, const int& pop_size, const double& cross_rate, const double& donor_rate);
private:
	int dim;
	int pop_size;
	int mnfes;
	int nfes;
	double cr; /*cross_rate*/
	double dr; /*donor_rate*/

	vector<vector<double>> population;        /*dim*pop_size �Ӹs���*/
	vector<vector<double>> donor_population;  /*�ܲ��᪺ donor ��*/
	vector<vector<double>> recent_population; /*crossover �᪺�s��*/
	vector<double> fit;                       /*��Ѫ� fitness*/
	vector<double> recent_fit;                /*�s�Ѫ� fitness*/
	double best_fit;                          /*�����̨�fitness*/
	vector<double> best_sol;                  /*�����̨θ�*/

	vector<double> fit_record;                    /*��X���ĵ��G��*/
	int r_count = 0;

	void Init();          /*���ͪ�ldim*pop_size�Ӹs���*/
	void Mutation();      /*����donor��*/
	void Crossover();     /*��� & donor�� ��e�ͦ�recent��*/
	void Evaluation();    /*�p��recent�Ѫ� fitness*/
	void Determination(); /*update��(��� or recent��)�i�����U�@�N��
						  �Aupdate best_fit*/
};

#endif
