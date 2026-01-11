#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include "Core/ParallelEvaluator.h"
#include "Core/Types.h"

int main() {
    // --- 測試目的說明 ---
    // 1. 正確性驗證 (Correctness)：確保平行計算出的路徑距離與序列版完全相同。
    // 2. 效能評估 (Performance)：測量 std::async 帶來的加速倍率。
    // 3. 記憶體安全：驗證 std::ref 是否正確將結果寫回原始 Individual 物件。

    const int CITY_COUNT = 2000;     // 城市數量
    const int POP_SIZE = 5000;      // 族群大小 (任務總量)
    
    std::cout << "--- Parallel Evaluator Unit Test ---" << std::endl;
    std::cout << "Config: City Count = " << CITY_COUNT << ", Population Size = " << POP_SIZE << std::endl;

    // 1. 準備測試資料
    // 建立一個對稱的距離矩陣 (假資料，假設所有點距離都是 1.5)
    std::vector<double> distMatrix(CITY_COUNT * CITY_COUNT, 1.5);
    
    // 初始化族群，並賦予每個個體相同的初始路徑
    std::vector<Individual> popParallel(POP_SIZE);
    for (auto& ind : popParallel) {
        ind.path.resize(CITY_COUNT);
        for (int i = 0; i < CITY_COUNT; ++i) ind.path[i] = i;
        ind.distance = 0.0; // 初始距離設為 0
    }

    // 複製一份一模一樣的族群給序列版使用
    std::vector<Individual> popSerial = popParallel;

    ParallelEvaluator evaluator;

    // 2. 執行序列版測試 (Baseline)
    auto startS = std::chrono::high_resolution_clock::now();
    evaluator.evaluate(popSerial, distMatrix, CITY_COUNT, false);
    auto endS = std::chrono::high_resolution_clock::now();
    double timeS = std::chrono::duration<double>(endS - startS).count();

    // 3. 執行平行版測試 (Target)
    auto startP = std::chrono::high_resolution_clock::now();
    evaluator.evaluate(popParallel, distMatrix, CITY_COUNT, true);
    auto endP = std::chrono::high_resolution_clock::now();
    double timeP = std::chrono::duration<double>(endP - startP).count();

    // 4. 輸出效能報告
    std::cout << "\n[Performance Report]" << std::endl;
    std::cout << "Serial Mode Time   : " << timeS << " seconds" << std::endl;
    std::cout << "Parallel Mode Time : " << timeP << " seconds" << std::endl;
    if (timeP > 0) {
        std::cout << "Speedup Ratio      : " << timeS / timeP << "x" << std::endl;
    }

    // 5. 比對結果準確性
    // 檢查平行版算出來的每一個 distance 是否都與序列版一致
    bool isMatch = true;
    for (int i = 0; i < POP_SIZE; ++i) {
        if (std::abs(popParallel[i].distance - popSerial[i].distance) > 1e-9) {
            std::cerr << "Verification FAILED: Result mismatch at index " << i << std::endl;
            std::cerr << "Serial: " << popSerial[i].distance << ", Parallel: " << popParallel[i].distance << std::endl;
            isMatch = false;
            break;
        }
    }

    if (isMatch) {
        std::cout << "\n[Verification SUCCESS]" << std::endl;
        std::cout << "The parallel evaluator results are identical to the serial version." << std::endl;
    } else {
        return -1; // 測試失敗
    }

    return 0;
}