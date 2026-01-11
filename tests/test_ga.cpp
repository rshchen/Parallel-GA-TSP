#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "GASolver.h"
#include "Types.h"

int main() {
    // --- 測試目的說明 ---
    // 1. 整合測試 (Integration Test)：驗證 GASolver 與 ParallelEvaluator 銜接後運作正常。
    // 2. 演算法正確性 (Algorithm Correctness)：利用正方形佈局驗證 GA 是否能找到理論最短路徑 40.0。
    // 3. 效能觀測：測量在開啟平行化模式下，完整演化流程的總執行時間。

    std::cout << "--- TSP Genetic Algorithm Final Integration Test ---" << std::endl;

    // 1. 定義 4 個城市，構成一個 10x10 的正方形
    // 理想路徑序列：0 -> 1 -> 2 -> 3 (或其對稱路徑)，總長度 40.0
    std::vector<City> cities = {
        {0, 0.0, 0.0},   // 城市 0 (左下)
        {1, 0.0, 10.0},  // 城市 1 (左上)
        {2, 10.0, 10.0}, // 城市 2 (右上)
        {3, 10.0, 0.0}   // 城市 3 (右下)
    };

    // 2. 設定 GA 參數
    GAConfig config;
    config.populationSize = 100;    // 增加族群大小以確保更穩定的收斂
    config.generations = 100;       // 演化 100 代
    config.mutationRate = 0.05;     // 5% 突變率
    config.cityCount = 4;
    config.tournamentSize = 5;      // 錦標賽選擇強度
    config.useParallel = true;      // 【關鍵】開啟並行化評估模式

    std::cout << "Configuration: " << std::endl;
    std::cout << "- Population Size: " << config.populationSize << std::endl;
    std::cout << "- Generations    : " << config.generations << std::endl;
    std::cout << "- Parallel Mode  : " << (config.useParallel ? "ON" : "OFF") << std::endl;

    // 3. 建立求解器並啟動計時
    GASolver solver(config, cities);
    
    std::cout << "\nStarting evolution..." << std::endl;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    Individual bestResult = solver.solve();
    auto endTime = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = endTime - startTime;

    // 4. 輸出結果與效能數據
    std::cout << "\n--- Evolution Finished ---" << std::endl;
    std::cout << "Execution Time: " << std::fixed << std::setprecision(6) << duration.count() << " seconds" << std::endl;
    std::cout << "Best Distance Found: " << bestResult.distance << std::endl;
    std::cout << "Optimal Distance Expected: 40.0" << std::endl;
    
    std::cout << "Best Path Found: ";
    for (size_t i = 0; i < bestResult.path.size(); ++i) {
        std::cout << bestResult.path[i] << (i == bestResult.path.size() - 1 ? "" : " -> ");
    }
    std::cout << " -> " << bestResult.path[0] << " (Back to start)" << std::endl;

    // 5. 斷言驗證
    // 考慮浮點數運算，使用極小值 epsilon 進行比對
    if (bestResult.distance <= 40.0001) {
        std::cout << "\n[TEST PASSED] The integrated solver successfully found the optimal path." << std::endl;
    } else {
        std::cout << "\n[TEST FAILED] The solver converged to a sub-optimal result: " << bestResult.distance << std::endl;
        return -1;
    }

    return 0;
}