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

	vector<vector<double>> population;        /*dim*pop_size 個群體解*/
	vector<vector<double>> donor_population;  /*變異後的 donor 解*/
	vector<vector<double>> recent_population; /*crossover 後的新解*/
	vector<double> fit;                       /*原解的 fitness*/
	vector<double> recent_fit;                /*新解的 fitness*/
	double best_fit;                          /*紀錄最佳fitness*/
	vector<double> best_sol;                  /*紀錄最佳解*/

	vector<double> fit_record;                    /*輸出收斂結果用*/
	int r_count = 0;

	void Init();          /*產生初始dim*pop_size個群體解*/
	void Mutation();      /*產生donor解*/
	void Crossover();     /*原解 & donor解 交叉生成recent解*/
	void Evaluation();    /*計算recent解的 fitness*/
	void Determination(); /*update誰(原解 or recent解)可成為下一代解
						  ，update best_fit*/
};

#endif
