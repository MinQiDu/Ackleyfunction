#ifndef JADE_H
#define JADE_H
#include "Ackley.h"

class algo_JADE
{
public:
	algo_JADE();       /* constructor 宣告 */
	void RunALG(const int& dim, const int& pop_size, const double& CR, const double& F, const double& c, const double& p);
private:
	random_device rd;  /*宣告亂數引擎*/
	mt19937 gen;

	int dim;
	int pop_size;
	int mnfes;
	int nfes;
	double mCR;        /*adaptive parameter 1 (cross_rate)*/
	double mF;         /*adaptive parameter 2 (= F, donor_rate)*/
	vector<double> CR; /*存此代每個個體的 CR*/
	vector<double> F;  /*存此代每個個體的 F*/
	double c;          /*adaptation rate*/
	double p;          /*top % pop chosen by pbest*/

	vector<double> sCR;                 /*存此代中成功更新的個體的CR*/
	vector<double> sF;                  /*存此代中成功更新的個體的F*/
	vector<vector<double>> A;           /*存歷代中被淘汰掉的原解，size = pop_size*/
	vector<vector<double>> pop;         /*dim*pop_size 個群體解*/
	vector<vector<double>> donor_pop;   /*變異後的 donor 解*/
	vector<vector<double>> current_pop; /*crossover 後的新解*/
	vector<double> fit;                 /*原解的 fitness*/
	vector<double> current_fit;         /*新解的 fitness*/
	double best_fit;                    /*紀錄最佳fitness*/
	vector<double> best_sol;            /*紀錄最佳解*/

	vector<double> fit_record;          /*輸出收斂結果用*/
	int r_count = 0;

	void Init();           /*產生初始dim*pop_size個群體解*/
	void Mutation();       /*產生donor解*/
	void Crossover();      /*原解 & donor解 交叉生成recent解*/
	void Evaluation();     /*計算recent解的 fitness*/
	void Determination();  /*update誰(原解 or recent解)可成為下一代解
						   ，update best_fit*/
	void ParaAdaptation(); /*更新 mCR & mF*/
};

#endif
