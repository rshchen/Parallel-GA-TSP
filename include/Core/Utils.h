#ifndef UTILS_H
#define UTILS_H

#include "Core/Types.h"
#include <random>

class Utils {
public:
    // 產生隨機城市 (用於測試或模擬)
    static std::vector<City> generateRandomCities(int n, double maxX, double maxY);

    // 預計算距離矩陣 (O(n^2))
    static std::vector<double> precomputeDistanceMatrix(const std::vector<City>& cities);

    // 取得預先計算好的距離 (O(1))
    static double getDistance(int cityA, int cityB, const std::vector<double>& distMatrix, int n);

    // 隨機整數產生 [min, max]
    static int getRandomInt(int min, int max);

    // 加上 min 和 max 參數，預設值分別為 0.0 和 1.0
    static double getRandomDouble(double min = 0.0, double max = 1.0);

    // 獲取靜態隨機數引擎的引用
    static std::mt19937& getGenerator() {
        return g_gen;
    }

private:
    // 靜態隨機數引擎，避免在函數內反覆建立物件導致隨機性失效
    static std::mt19937 g_gen;
};

#endif