#ifndef PARALLEL_EVALUATOR_H
#define PARALLEL_EVALUATOR_H

#include "Core/Types.h"
#include <vector>

/**
 * @class ParallelEvaluator
 * @brief 高效能路徑評估器
 * * 本類別負責族群中所有個體的適應度（路徑距離）計算。
 * 採用任務導向的平行處理架構 (Task-based Parallelism)，利用 C++17 的 std::async 
 * 將運算密集型任務分發至多核 CPU，以極大化吞吐量。
 */
class ParallelEvaluator {
public:
    /**
     * @brief 執行全族群的路徑評估
     * * 根據 useParallel 參數決定執行模式。在平行模式下，系統會將每個個體的計算
     * 封裝為非同步任務，實現高效的負載平衡。
     * * @param population 待評估的族群引用
     * @param distMatrix 預計算的扁平化距離矩陣 (用於 O(1) 查表)
     * @param cityCount 城市總數
     * @param useParallel 是否啟用 std::async 並行處理
     */
    void evaluate(std::vector<Individual>& population, 
                  const std::vector<double>& distMatrix, 
                  int cityCount,
                  bool useParallel);

private:
    /**
     * @brief 單個個體的路徑計算核心邏輯
     * * 此為純運算密集型 (CPU Bound) 函式，計算給定路徑的總歐幾里得距離。
     * 透過扁平化矩陣訪問優化快取命中率。
     * * @param ind 欲計算的個體引用
     * @param distMatrix 距離查表矩陣
     * @param cityCount 城市總數
     */
    void evaluateIndividual(Individual& ind, const std::vector<double>& distMatrix, int cityCount);
};

#endif