#include <iostream>
#include <vector>
#include "GASolver.h"
#include "Types.h"

int main() {
    std::cout << "--- TSP Genetic Algorithm Test: Square Layout ---" << std::endl;

    // 1. 定義 4 個城市，構成一個 10x10 的正方形
    // 理論最短路徑應該是 10 * 4 = 40.0
    std::vector<City> cities = {
        {0, 0.0, 0.0},   // 城市 0
        {1, 0.0, 10.0},  // 城市 1
        {2, 10.0, 10.0}, // 城市 2
        {3, 10.0, 0.0}   // 城市 3
    };

    // 2. 設定 GA 參數
    GAConfig config;
    config.populationSize = 20;    // 族群不必大，因為城市很少
    config.generations = 50;       // 演化 50 代
    config.mutationRate = 0.1;     // 10% 突變率
    config.cityCount = 4;
    config.tournamentSize = 3;     // 錦標賽 3 人選 1

    // 3. 建立求解器並執行
    GASolver solver(config, cities);
    
    std::cout << "Starting evolution..." << std::endl;
    Individual bestResult = solver.solve();

    // 4. 輸出結果
    std::cout << "\n--- Evolution Finished ---" << std::endl;
    std::cout << "Best Distance Found: " << bestResult.distance << std::endl;
    std::cout << "Optimal Distance Expected: 40.0" << std::endl;
    
    std::cout << "Best Path: ";
    for (int cityId : bestResult.path) {
        std::cout << cityId << " -> ";
    }
    std::cout << bestResult.path[0] << " (Back to start)" << std::endl;

    // 5. 簡單的斷言驗證 (如果距離 <= 40 則通過)
    if (bestResult.distance <= 40.0001) {
        std::cout << "\n[TEST PASSED] The solver found the optimal square path!" << std::endl;
    } else {
        std::cout << "\n[TEST FAILED] The solver did not converge. Check your operators." << std::endl;
    }

    return 0;
}