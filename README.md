# Implement PSO & DE to solve AckleyFunction

##  1. Introduction
- Using C++
- 使用 PSO 和 DE 解 Ackley function
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
- void RunALG(const int& dim, const int& pop_size, const double& cross_rate, const double& donor_rate)  
/*執行DE*/

- void Init();          
/*產生初始dim*pop_size個群體解*/

- void Mutation();      
/*產生donor解*/

- void Crossover();     
/*原解 & donor解 交叉生成recent解*/

- void Evaluation();    
/*計算recent解的 fitness*/

- void Determination(); 
/*update誰(原解 or recent解)可成為下一代解，update best_fit*/

## 3. Input
### Command_line arguments :
- 解空間維度 | dim = 2 or 10
- 單次生成解次數 | pop_size = 100
- (for DE) Cross_rate 交叉生成率 | cr = 0.8
- (for DE) Donor_rate 生成donor解比重 | dr = 0.8

## 4. Output
### `PSO`
- fitness_PSO_dim(2 or 10)_pop100.txt
- plot_PSO.plt
- result_Ackley_PSO_dim(2 or 10)_pop100.png
### `DE`
- fitness_DE_dim(2 or 10)_pop100.txt
- plot_DE.plt
- result_Ackley_DE_dim(2 or 10)_pop100.png

## 5. 執行方式     
### `------------編譯------------` 
#### `VisualStudio`
1. 開啟 Visual Studio 專案 `ackleyfunction.sln`
2. 按下 `Ctrl + F5` 開始編譯

#### `VSCode`
1. 在 Windows cmd 或 PowerShell 中
2. 先到正確資料夾路徑
3. 輸入：   
g++ main.cpp DE.cpp PSO.cpp Ackley.cpp -o ackleyfunction.exe  

### `------------執行------------`
1. 在 Windows cmd 或 PowerShell 中
2. 先到正確資料夾路徑
3. 輸入：   
.\ackleyfunction.exe `dim = 2 or 10` `pop_size = 100` `cr = 0.8` `dr = 0.8` 
出現 :
Choose Algorithm ( PSO / DE )
輸入演算法 ( PSO or DE )

##  6. 檔案結構
- ackleyfunction/  
 `main.cpp`  
 `PSO.cpp` / `PSO.h`  
 `DE.cpp` / `DE.h`   
 `Ackley.cpp` / `Ackley.h`
