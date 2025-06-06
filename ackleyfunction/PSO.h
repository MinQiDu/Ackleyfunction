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

	void Init();          /*產生初始 pop_size 個 dim 維度的解存入陣列 populatio*/ /*初始化 personal_best& global_best為此初始解中 fitness 最優的，計算初始 velocity* /
	/*while nfes < mnfes*/
	void Transition();    /*產生新的 Velocity + 原解 = 新解，更新到population*/
	void Evaluation();    /*計算新解 fitness*/
	void Determination(); /*更新 personal_best & global_best*/
};

#endif