#include "Core/Utils.h"
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
    std::cout << "--- Running Utils Test ---" << std::endl;

    int n = 50;
    auto cities = Utils::generateRandomCities(n, 1000.0, 1000.0);

    // 1. 驗證城市座標範圍
    for (const auto& c : cities) {
        assert(c.x >= 0 && c.x <= 1000.0);
        assert(c.y >= 0 && c.y <= 1000.0);
    }
    std::cout << "[Step 1] City Generation: SUCCESS" << std::endl;

    // 2. 驗證距離矩陣
    auto matrix = Utils::precomputeDistanceMatrix(cities);
    
    // 測試對稱性與對角線
    for (int i = 0; i < n; ++i) {
        assert(std::abs(Utils::getDistance(i, i, matrix, n)) < 1e-9); // 對角線應為 0
        for (int j = 0; j < n; ++j) {
            double d1 = Utils::getDistance(i, j, matrix, n);
            double d2 = Utils::getDistance(j, i, matrix, n);
            assert(std::abs(d1 - d2) < 1e-9); // 必須對稱
        }
    }
    std::cout << "[Step 2] Matrix Symmetry: SUCCESS" << std::endl;

    // 3. 測試隨機整數範圍
    for (int i = 0; i < 100; ++i) {
        int r = Utils::getRandomInt(10, 20);
        assert(r >= 10 && r <= 20);
    }
    std::cout << "[Step 3] Randomization: SUCCESS" << std::endl;

    std::cout << "All Utils tests passed!" << std::endl;
    return 0;
}