# Implement PSO & DE & JADE to solve AckleyFunction

##  1. Introduction
- Using C++
- 使用 PSO 和 DE 和 JADE 解 Ackley function
- 支援dimension、population 數量調整

## 2. Main Function
### `ParticalSwarmOptimization (PSO)`
- void RunALG(int dim, int pop_size)   
/*執行PSO*/

- void Init()           
/*產生初始 pop_size 個 dim 維度的解存入陣列 populatio，初始化 personal_best& global_best*/

- void Transition()     
/*產生新的 Velocity + 原解 = 新解，更新到population*/

- void Evaluation();    
/*計算新解 fitness*/

- void Determination();   
/*更新 personal_best & global_best*/

### `DifferentialEvolution (DE)`
- void RunALG(const int& dim, const int& pop_size, const double& CR, const double& F)  
/*執行DE*/

- void Init();          
/*產生初始dim*pop_size個群體解*/

- void Mutation();      
/*使用 "DE/rand/1" 產生donor解*/

- void Crossover();     
/*原解 & donor解 交叉生成current解*/

- void Evaluation();    
/*計算current解的 fitness*/

- void Determination(); 
/*update誰(原解 or current解)可成為下一代解，update best_fit*/

### `AdaptiveDifferentialEvolution with ExternalArchive (JADE)`
- void RunALG(const int& dim, const int& pop_size, const double& CR, 
const double& F, const double& _c, const double& _p)  
/*執行JADE*/

- void Init();          
/*產生初始dim*pop_size個群體解*/

- void Mutation();      
/*使用 "DE/current-to-pbest/1" 產生donor解*/

- void Crossover();     
/*原解 & donor解 交叉生成current解*/

- void Evaluation();    
/*計算current解的 fitness*/

- void Determination();    
/*update誰(原解 or current解)可成為下一代解，update best_fit*/

- void ParaAdaptation();   
/*更新 mCR & mF*/

## 3. Input
### Command_line arguments :
- 解空間維度 | dim = 2 or 10 or 30 (觀察高維度)
- 單次生成解次數 | pop_size = 100
- (for DE & JADE) Cross_rate 交叉生成率 | CR = 0.8   
/*JADE預設CR = 0.5*/
- (for DE & JADE) Donor_rate 生成donor解比重 | F = 0.8   
/*JADE預設F = 0.5*/
- (for JADE) Adaptation rate 更新參數的比重 | c = 0.05 ~ 0.2
- (for JADE) Top p% pop 隨機選出 x_pbest | p = 0.05 ~ 0.2

## 4. Output
### `PSO`
- fitness_PSO_dim(2 or 10)_pop100.txt
- plot_PSO.plt
- result_Ackley_PSO_dim(2 or 10)_pop100.png
### `DE`
- fitness_DE_dim(2 or 10)_pop100_CR9_F5.txt   
/*CR9表示CR = 0.9, F5表示F = 0.5*/
- plot_DE.plt
- result_Ackley_DE_dim(2 or 10)_pop100_CR9_F5.png
### `JADE`
- fitness_JADE_dim(2 or 10)_pop100_c20_p20.txt   
/*c20表示c = 0.2, p20表示p = 0.2*/
- plot_JADE.plt
- result_Ackley_JADE_dim(2 or 10)_pop100_c20_p20.png

## 5. 執行方式     
### `------------編譯------------` 
#### `VisualStudio`
1. 開啟 Visual Studio 專案 `ackleyfunction.sln`
2. 按下 `Ctrl + F5` 開始編譯

#### `VSCode`
1. 在 Windows cmd 或 PowerShell 中
2. 先到正確資料夾路徑
3. 輸入：   
g++ main.cpp DE.cpp JADE.cpp PSO.cpp Ackley.cpp -o ackleyfunction.exe  

### `------------執行------------`
1. 在 Windows cmd 或 PowerShell 中
2. 先到正確資料夾路徑
3. 輸入：   
.\ackleyfunction.exe `dim = 2 or 10` `pop_size = 100` `CR = 0.9` `F = 0.5`    
出現 :   
Choose Algorithm ( PSO / DE / JADE)   
輸入演算法 ( PSO or DE or JADE)   
選JADE接著出現:   
Please enter the value of adaptation rate c = 輸入C值   
Please enter the value of the top % pop chosen for pbest = 輸入p值

##  6. 檔案結構
- ackleyfunction/  
 `main.cpp`  
 `PSO.cpp` / `PSO.h`  
 `DE.cpp` / `DE.h`   
 `JADE.cpp` / `JADE.h`    
 `Ackley.cpp` / `Ackley.h`
