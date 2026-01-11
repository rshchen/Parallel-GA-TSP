#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <algorithm>
#include <cmath>

/**
 * @brief 城市基礎資料結構
 * 封裝 ID 與 二維空間座標
 */
struct City {
    int id;
    double x;
    double y;
};

/**
 * @brief 遺傳演算法中的個體（染色體）
 * 代表一個潛在的路徑解
 */
struct Individual {
    std::vector<int> path; // 儲存城市 ID 的置換排列 (Permutation)
    double distance;       // 該路徑的總長度
    double fitness;        // 適應度 (通常定義為 1/distance)

    Individual() : distance(0.0), fitness(0.0) {}

    /**
     * @brief 排序運算子重載
     * 用於精英保留策略：適應度越高（距離越短）者排在前面
     */
    bool operator<(const Individual& other) const {
        return fitness > other.fitness;
    }
};

/**
 * @brief 遺傳演算法參數配置結構
 * 體現 n-dependent 的動態參數調整邏輯
 */
struct GAConfig {
    int cityCount;          // 城市規模 n
    int populationSize;     // 群體規模 P ≈ 4n
    int generations;        // 最大演化代數 G ≈ 100n
    double crossoverRate;   // 交配機率 (建議 0.8 - 0.9)
    double mutationRate;    // 突變機率 p_m = 1/n
    int tournamentSize;     // 錦標賽競爭人數 k ≈ 5% P
    int eliteCount;         // 精英保留人數 (建議 2-5%)
    bool useParallel;      // 是否啟用並行評估

    /**
     * @brief 靜態工廠方法：根據城市數量自動生成優化參數
     * 實踐「參數隨規模動態調整」的研發思維
     */
    static GAConfig generateDefault(int n) {
        GAConfig config;
        config.cityCount = n;
        
        // P ≈ 4n, 且設定下限確保多樣性
        config.populationSize = std::max(100, 4 * n);
        
        // G ≈ 100n
        config.generations = 100 * n;
        
        config.crossoverRate = 0.85;
        
        // 核心法則：期望突變次數為 1
        config.mutationRate = 1.0 / static_cast<double>(n);
        
        // k ≈ 5% P
        config.tournamentSize = std::max(2, static_cast<int>(0.05 * config.populationSize));
        
        // 精英保留 ≈ 2-5%
        config.eliteCount = std::max(1, static_cast<int>(0.03 * config.populationSize));
        
        return config;
    }
};

#endif // TYPES_H