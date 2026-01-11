#include "Core/ParallelEvaluator.h"
#include <future>
#include <thread>
#include <algorithm>


void ParallelEvaluator::evaluate(std::vector<Individual>& population, 
                                 const std::vector<double>& distMatrix, 
                                 int cityCount,
                                 bool useParallel) {
    if (useParallel && population.size() > 64) {
        // --- [模式 A] 批次並行處理 (Batched std::async) ---
        
        // 1. 獲取硬體支援的執行緒數量 (標準 C++11)
        unsigned int numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0) numThreads = 2; // 保底機制

        int totalSize = population.size();
        int batchSize = totalSize / numThreads;
        std::vector<std::future<void>> futures;

        for (unsigned int i = 0; i < numThreads; ++i) {
            int startIdx = i * batchSize;
            int endIdx = (i == numThreads - 1) ? totalSize : (i + 1) * batchSize;

            // 確保該區段有效
            if (startIdx >= endIdx) break;

            // 使用 std::async 啟動批次任務
            // 這裡我們改用 Lambda 函式來包裝「一段路徑」的計算
            // 1. std::launch::async: 強制開啟新執行緒執行
            // 2. [this, &population, &distMatrix]: 捕獲列表，傳入 this 指標以呼叫成員函式
            // 3. 透過引用 (&) 傳遞大資料 (population, distMatrix)，效能等同 std::ref
            futures.push_back(std::async(std::launch::async, [this, &population, &distMatrix, cityCount, startIdx, endIdx]() {
                for (int j = startIdx; j < endIdx; ++j) {
                    // 呼叫成員函式：這裡同樣需要 this 指標來執行 evaluateIndividual
                    // 並且操作的是 population[j] 的引用
                    this->evaluateIndividual(population[j], distMatrix, cityCount);
                }
            }));
        }

        // 等待所有執行緒計算完畢 (Blocking Wait)
        for (auto& f : futures) {
            f.get();
        }
    } else {
        // --- [模式 B] 序列處理 (Serial Evaluation) ---
        for (auto& ind : population) {
            evaluateIndividual(ind, distMatrix, cityCount);
        }
    }
}

void ParallelEvaluator::evaluateIndividual(Individual& ind, 
                                           const std::vector<double>& distMatrix, 
                                           int cityCount) {
    double totalDist = 0.0;
    
    // 計算路徑中相鄰城市的距離之和
    for (int i = 0; i < cityCount - 1; ++i) {
        int from = ind.path[i];
        int to = ind.path[i + 1];
        // 透過扁平化索引進行 O(1) 查表
        totalDist += distMatrix[from * cityCount + to];
    }

    // 最後要加上「回到起點」的距離，形成一個封閉迴圈
    int last = ind.path[cityCount - 1];
    int first = ind.path[0];
    totalDist += distMatrix[last * cityCount + first];

    // 更新個體屬性
    ind.distance = totalDist;
    // 適應度通常與距離成反比，這裡加上 1.0 是為了防止除以零的極端情況
    ind.fitness = 1.0 / (totalDist + 1.0);
}