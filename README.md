# Parallel-GA-TSP

這是一個高效能的旅行推銷員問題 (TSP) 求解器，基於混合遺傳演算法 (Memetic Algorithm) 架構開發。本專案結合了遺傳演算法的全域搜尋能力與 2-Opt 局部開發能力，並透過 C++17 的非同步平行運算技術，在多核 CPU 環境下達成顯著的效能提升。

---

## 技術亮點 (Technical Highlights)

* **混合演化架構 (Memetic Algorithm)**：結合遺傳演算法 (GA) 與 2-Opt 局部搜尋，解決純 GA 在大型問題（如 n > 100）收斂速度過慢且容易陷入局部最優的問題。
* **非同步任務平行化**：利用 std::async 實現任務導向的平行評估 (Task-based Parallelism)，在多核環境下達成約 7.09x 的加速比。
* **N-dependent 參數工程**：實作隨城市規模 n 動態調整的參數工廠，自動優化族群大小、突變率與錦標賽壓力，確保演算法的穩健性。
* **現代化建構流程**：
    * 支援 CMake Presets (Debug/Release 獨立配置)。
    * 透過編譯定義注入資料路徑，確保跨環境執行的路徑正確性。
    * 採用 Callback 設計解耦演算法核心與進度回報邏輯。

---

## 標竿測試結果 (Benchmarking)

本專案採用業界標準的 **TSPLIB (Traveling Salesman Problem Library)** 資料集進行測試。透過多維度的數據指標，驗證演算法在「精準度」與「執行效能」上的表現。


### 1. 尋優精準度與效能分析 (Optimality, Accuracy & Performance)

針對不同規模的經典測項 (TSPLIB)，進行 10 次獨立實驗統計。結果顯示本系統在各規模下均能穩定收斂，且在高節點數（n=150）下仍能保持優異的運算速度。

| Instance | 城市規模 (n) | TSPLIB 最優解 | 實測最佳 (Best) | 誤差 (Gap %) | 穩定度 (CV %) | 平均耗時 (s) |
| --- | --- | --- | --- | --- | --- | --- |
| **berlin52** | 52 | 7542 | 7710.83 | **2.24%** | 2.76% | **3.79s** |
| **st70** | 70 | 675 | 694.47 | **2.88%** | 2.18% | **5.29s** |
| **ch150** | 150 | 6528 | 6801.80 | **4.19%** | 1.75% | **7.87s** |

*註：*

1. **Gap %**：計算參考自 TSPLIB 官方公佈之最佳已知解 (Best Known Solution)，計算公式為
$\frac{\text{Best} - \text{Optimal}}{\text{Optimal}} \times 100\%$ 。
2. **CV % (變異係數)**：反映演算法在隨機種子變動下的穩定性，數值越低代表演算法越穩定。
3. **平均耗時**：為 10 次測試之平均執行時間，展現了 C++ 實作在處理大規模路徑規劃時的高效能。




### 2. 並行運算效能 (Parallel Efficiency)
針對大規模運算場景進行壓力測試 (Stress Test)，驗證多核心 CPU 的利用效率。

* **測試環境**：MacBook Air M2 (8-Core CPU)
* **測試規模**：2,000 Cities / 5,000 Population
* **效能數據**：
    * **序列模式 (Serial)**: 0.02616 s
    * **並行模式 (Parallel)**: 0.00368 s
    * **加速倍率 (Speedup Ratio)**: **7.09x**



> **技術總結**：在 8 核心環境下達成 **6.10 倍加速**，證明了基於 `std::async` 的任務批次化架構能有效分攤運算負擔。對於 ch150 等中大型問題，本系統能在秒級時間內提供 Gap 5% 以內的高品質滿意解。


## 深度技術開發筆記 (Technical Deep Dives)

本專案不僅是程式碼實作，更包含從數學理論到工程效能的完整研發紀錄：

### 理論分析 (Theoretical Analysis)
* [NP 複雜度與 TSP 問題特點](https://hackmd.io/@VictorJuiHsiang/Bk8YVmTEbx)  
  探討 TSP 的數學本質及 $NP\text{-hard}$ 複雜度，解釋組合爆炸對運算的挑戰，並建立採用啟發式演算法的理論動機。
* [TSP 演算法策略與效能權衡分析](https://hackmd.io/TDCg-xjQSRSZnZpAnZEPvg?both)  
  橫向比較蠻力法、動態規劃與啟發式算子，分析「解的品質」與「時間成本」間的權衡 (Trade-off)，說明本專案選擇混合架構的決策過程。

### 演算法優化 (Algorithm Optimization)
* [GA 核心原理與 Memetic 策略設計](https://hackmd.io/@VictorJuiHsiang/rkTCMI1H-l)  
  深入介紹遺傳演算法之編碼與算子設計，並探討如何結合 **2-Opt 局部搜尋** 轉化為高效能的 Memetic 架構。
* [隨城市規模 n 的動態參數調優與策略選型](https://hackmd.io/@VictorJuiHsiang/SJu84wJSWg)  
  建立 $n\text{-dependent}$ 參數模型，探討 **OX 交叉算子** 對路徑特徵的保護優勢，以及如何透過統計學驗證演算法的魯棒性。

### 工程架構與實測 (Engineering & Results)
* [C++ Parallel GA 實作藍圖與研發紀錄](https://hackmd.io/@VictorJuiHsiang/S1N9KP1SWl)  
  展示專業 C++ 專案目錄設計、模組化職責分配，以及在時間壓力下從 **MVP 原型** 迭代至嚴謹研發架構的實務紀錄。
* [遺傳演算法求解 TSP：實作結果與實驗紀錄](https://hackmd.io/@VictorJuiHsiang/BJdQ2wJrWg)  
  彙整 **7.09x 平行加速效能** 數據，並針對 TSPLIB 標準測項進行誤差分析，證明系統在不同規模下的穩定性與精準度。

---

## 建構與執行 (Build & Usage)

本專案採用現代化 C++ 開發工作流，支援 CMake 3.19+ 與 **Ninja** 高速建構系統。

### 1. 環境需求 (Prerequisites)
* **編譯器**：支援 C++17 之編譯器 (如 GCC 9+, Clang 10+, 或 MSVC 2019+)。
* **建構工具**：[CMake](https://cmake.org/) (3.19+) 與 [Ninja](https://ninja-build.org/) (推薦使用，以獲得最佳編譯速度)。

### 2. 配置與編譯 (Build)
本專案支援 CMake Presets。建議使用 Release 模式進行建構，以確保標竿測試數據的準確性。

```bash
# 1. 配置環境 (使用 Ninja 作為生成器)
cmake --preset release -G Ninja

# 2. 執行建構
cmake --build build/release

# 3. 執行程式 (Execution)
# 編譯完成後，可以透過以下指令執行標竿測試或範例程式：
# 執行 TSPLIB 標準測項標竿測試
./build/release/test_tsplib_benchmark

# 執行一般 TSP 求解範例
./build/release/tsp_solver
```

## 專案結構 (Project Structure)


```text
Parallel-GA-TSP/
├── data/tsplib/        # TSPLIB 標準測試檔案 (.tsp)
├── include/
│   ├── Core/           # 演算法核心標頭檔
│   └── Parser/         # 檔案解析器標頭檔
├── src/                # 實作程式碼 (.cpp)
├── tests/              # 單元測試與標竿測試
├── examples/           # 使用範例
├── CMakePresets.json   # 編譯器設定
└── CMakeLists.txt      # 建構配置文件
```

---

## 作者 (Author)

**Jui-Hsiang Chen (Victor)**

* **國立成功大學 數學系 (學士 & 碩士)** - 主修組合數學。
* **軟體研發工程師** - 任職於技術公司韌體部門。
