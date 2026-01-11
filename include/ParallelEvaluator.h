#ifndef PARALLEL_EVALUATOR_H
#define PARALLEL_EVALUATOR_H

#include "Types.h"
#include <vector>

class ParallelEvaluator {
public:
    /**
     * @brief 執行路徑評估
     * @param useParallel 是否啟用 std::async 並行處理
     */
    void evaluate(std::vector<Individual>& population, 
                                 const std::vector<double>& distMatrix, 
                                 int cityCount,
                                 bool useParallel);

private:
    /**
     * @brief 單個個體的路徑計算邏輯
     * 這是純運算密集型 (CPU Bound) 的部分，最適合並行化。
     */
    void evaluateIndividual(Individual& ind, const std::vector<double>& distMatrix, int cityCount);
};

#endif