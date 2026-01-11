#include "Core/Utils.h"
#include <cmath>
#include <ctime>

// 初始化靜態引擎 (使用當前時間作為種子)
std::mt19937 Utils::g_gen(static_cast<unsigned int>(std::time(nullptr)));

std::vector<City> Utils::generateRandomCities(int n, double maxX, double maxY) {
    std::vector<City> cities;
    cities.reserve(n);
    std::uniform_real_distribution<double> distX(0, maxX);
    std::uniform_real_distribution<double> distY(0, maxY);

    for (int i = 0; i < n; ++i) {
        cities.push_back({i, distX(g_gen), distY(g_gen)});
    }
    return cities;
}

std::vector<double> Utils::precomputeDistanceMatrix(const std::vector<City>& cities) {
    int n = static_cast<int>(cities.size());
    std::vector<double> matrix(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                matrix[i * n + j] = 0.0; // 對角線為 0
            } else {
                double dx = cities[i].x - cities[j].x;
                double dy = cities[i].y - cities[j].y;
                matrix[i * n + j] = std::sqrt(dx * dx + dy * dy);
            }
        }
    }
    return matrix;
}

double Utils::getDistance(int cityA, int cityB, const std::vector<double>& distMatrix, int n) {
    // 數學映射：(row, col) -> index
    return distMatrix[cityA * n + cityB];
}

int Utils::getRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(g_gen);
}

double Utils::getRandomDouble(double min, double max) {
    // 根據傳入的參數決定分佈區間
    std::uniform_real_distribution<double> dist(min, max);
    
    // 使用你原本定義好的 g_gen (隨機數引擎)
    return dist(getGenerator()); 
}