#ifndef PSO_H
#define PSO_H
#include "Ackley.h"

class algo_PSO
{
public:
	void RunALG(int dim, int pop_size);
private:
	int dim;              /*dimension*/
	int pop_size;         /*population of each run*/
	//int run;            /*run = (dim * 10000)/pop_size*/
	int mnfes;            /*max nums of evaluation so far*/
	int nfes;             /*nums of evaluation so far*/

	vector<vector<double>> population;
	vector<vector<double>> velocity;
	vector<vector<double>> p_best;
	vector<double> p_recent_fit;
	vector<double> p_best_fit;
	vector<double> g_best;
	double g_best_fit;

	vector<double> fit_record;
	int r_count;

	void Init();          /*���ͪ�l pop_size �� dim ���ת��Ѧs�J�}�C populatio*/ /*��l�� personal_best& global_best������l�Ѥ� fitness ���u���A�p���l velocity* /
	/*while nfes < mnfes*/
	void Transition();    /*���ͷs�� Velocity + ��� = �s�ѡA��s��population*/
	void Evaluation();    /*�p��s�� fitness*/
	void Determination(); /*��s personal_best & global_best*/
};

#endif