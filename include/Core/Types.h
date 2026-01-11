#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>

/**
 * @struct City
 * @brief 城市基礎資料結構
 * * 封裝城市唯一識別碼 (ID) 與其在二維空間中的幾何座標 (Cartesian coordinates)。
 */
struct City {
    int id;       /**< 城市唯一編號 */
    double x;     /**< X 軸座標 */
    double y;     /**< Y 軸座標 */
};

/**
 * @struct Individual
 * @brief 遺傳演算法中的個體 (染色體)
 * * 代表旅行推銷員問題的一個潛在路徑解。
 * * 內部以「置換排列 (Permutation)」的形式儲存城市存取順序。
 */
struct Individual {
    std::vector<int> path; /**< 城市 ID 的排列序列 */
    double distance;       /**< 該路徑的總歐幾里得距離 */
    double fitness;        /**< 適應度評分 (通常定義為 $1/distance$) */

    /**
     * @brief 預設建構子
     */
    Individual() : distance(0.0), fitness(0.0) {}

    /**
     * @brief 排序運算子重載
     * * 用於精英保留策略與族群排序。根據適應度進行降冪排序，
     * 確保適應度越高（距離越短）的個體排在集合前端。
     * @param other 比較對象
     * @return 若當前個體較優則返回 true
     */
    bool operator<(const Individual& other) const {
        return fitness > other.fitness;
    }
};

/**
 * @struct GAConfig
 * @brief 遺傳演算法參數配置結構
 * * 本結構實踐了「隨規模動態調整 (N-dependent)」的研發思維，
 * 旨在為不同維度的 TSP 問題提供穩健的啟發式參數預設值。
 */
struct GAConfig {
    int cityCount;          /**< 城市規模 $n$ */
    int populationSize;     /**< 族群規模 $P$ (建議 $\approx 4n$) */
    int generations;        /**< 最大演化代數 $G$ (建議 $\approx 100n$) */
    double crossoverRate;   /**< 交配機率 (建議 0.8 - 0.9) */
    double mutationRate;    /**< 突變機率 $p_m$ (建議 $1/n$) */
    int tournamentSize;     /**< 錦標賽競爭人數 $k$ (配合 2-Opt 建議採用弱選擇壓力 $k=3$) */
    int eliteCount;         /**< 精英保留人數 (建議 2-5% $P$) */
    bool useParallel;       /**< 是否啟用 std::async 多執行緒評估 */

    /** * @brief 演化進度回報回呼函式
     * 格式：void(當前代數, 當前最佳距離)
     */
    std::function<void(int, double)> onGenerationComplete = nullptr;

    /**
     * @brief 靜態工廠方法：根據城市數量自動生成優化參數
     * * 實踐「參數隨規模動態調整」的研發思維。
     * * 在 Memetic 架構下，為了配合強大的局部搜尋 (2-Opt)，將錦標賽規模固定為較小值以維持基因多樣性。
     * @param n 城市數量
     * @return 配置完成的 GAConfig 實例
     */
    static GAConfig generateDefault(int n) {
        GAConfig config;
        config.cityCount = n;
        
        // P ≈ 4n，設定下限以確保搜尋空間覆蓋率
        config.populationSize = std::max(100, 4 * n);
        
        // G ≈ 100n，確保在大問題規模下有足夠的演化時間
        config.generations = 100 * n;
        
        config.crossoverRate = 0.85;
        
        // 核心法則：確保每一條染色體平均發生 1 次突變
        config.mutationRate = 1.0 / static_cast<double>(n);
        
        // 固定小規模錦標賽：降低選擇壓力，將「精煉」職責交給 2-Opt，GA 則負責廣域探索
        config.tournamentSize = 3; 
        
        // 精英保留 ≈ 3%
        config.eliteCount = std::max(1, static_cast<int>(0.03 * config.populationSize));
        
        config.useParallel = true;
        return config;
    }
};

#endif // TYPES_H