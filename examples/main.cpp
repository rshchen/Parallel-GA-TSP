#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "Core/GASolver.h"
#include "Core/Types.h"
#include "Core/Utils.h"

/**
 * @brief 產生隨機城市資料
 * @param count 城市數量
 * @param maxWidth 座標最大寬度
 * @param maxHeight 座標最大高度
 * @return 隨機生成的城市列表
 */
std::vector<City> generateRandomCities(int count, double maxWidth, double maxHeight) {
    std::vector<City> cities;
    cities.reserve(count);
    for (int i = 0; i < count; ++i) {
        cities.push_back({
            i, 
            Utils::getRandomDouble(0, maxWidth), 
            Utils::getRandomDouble(0, maxHeight)
        });
    }
    return cities;
}

int main() {
    // --- 範例程式目的 ---
    // 1. 示範如何配置 GA 參數。
    // 2. 展示隨機問題的生成與求解過程。
    // 3. 測量並輸出實際運行效能。

    // 1. 設定問題規模
    const int CITY_COUNT = 50;
    const int POPULATION_SIZE = 1000;
    const int GENERATIONS = 500;

    std::cout << "===========================================" << std::endl;
    std::cout << "   TSP Parallel GA Solver - Example Demo   " << std::endl;
    std::cout << "===========================================" << std::endl;

    // 2. 準備城市數據
    std::vector<City> cities = generateRandomCities(CITY_COUNT, 1000.0, 1000.0);
    std::cout << "[Info] Generated " << CITY_COUNT << " random cities." << std::endl;

    // 3. 設定演算法參數
    GAConfig config;
    config.cityCount = CITY_COUNT;
    config.populationSize = POPULATION_SIZE;
    config.generations = GENERATIONS;
    config.mutationRate = 0.02;    // 2% 突變率
    config.tournamentSize = 10;    // 錦標賽規模
    config.useParallel = true;     // 開啟並行化評估

    std::cout << "[Info] Population Size: " << config.populationSize << std::endl;
    std::cout << "[Info] Generations    : " << config.generations << std::endl;
    std::cout << "[Info] Parallel Mode  : " << (config.useParallel ? "Enabled" : "Disabled") << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    // 4. 執行求解器
    GASolver solver(config, cities);
    
    std::cout << "Starting evolution..." << std::endl;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    Individual bestResult = solver.solve();
    auto endTime = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = endTime - startTime;

    // 5. 輸出最終結果
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Evolution Completed!" << std::endl;
    std::cout << "Total Time Execution: " << std::fixed << std::setprecision(4) << duration.count() << " s" << std::endl;
    std::cout << "Best Distance Found : " << std::fixed << std::setprecision(2) << bestResult.distance << std::endl;

    // 輸出前 10 個城市的路徑片段作為示意
    std::cout << "Best Path (First 10): ";
    for (int i = 0; i < std::min(10, (int)bestResult.path.size()); ++i) {
        std::cout << bestResult.path[i] << " -> ";
    }
    std::cout << "..." << std::endl;
    std::cout << "===========================================" << std::endl;

    return 0;
}