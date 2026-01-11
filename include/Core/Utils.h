#ifndef UTILS_H
#define UTILS_H

#include "Core/Types.h"
#include <random>

/**
 * @class Utils
 * @brief 通用工具類別
 * * 提供包括隨機數生成、城市資料模擬以及高效能距離矩陣運算等輔助功能。
 * 所有成員均為靜態 (static)，作為全域工具組使用。
 */
class Utils {
public:
    /**
     * @brief 產生隨機城市資料
     * * 用於模擬測試或演算法基準測試，產生指定範圍內的隨機二維座標。
     * @param n 欲產生的城市數量
     * @param maxX X 軸最大邊界
     * @param maxY Y 軸最大邊界
     * @return 隨機產生的城市向量集合
     */
    static std::vector<City> generateRandomCities(int n, double maxX, double maxY);

    /**
     * @brief 預計算距離矩陣
     * * 為了消除演化過程中重複的三角函數與開根號運算，預先計算所有城市間的歐幾里得距離。
     * 採用扁平化一維向量存儲以提升記憶體存取連續性 (Spatial Locality)，時間複雜度為 $O(n^2)$。
     * @param cities 城市座標列表
     * @return 扁平化的距離矩陣向量
     */
    static std::vector<double> precomputeDistanceMatrix(const std::vector<City>& cities);

    /**
     * @brief 獲取預先計算好的距離 (查表優化)
     * * 使用 $O(1)$ 的時間複雜度獲取兩城市間的距離。
     * 索引公式：$index = i \times n + j$
     * @param cityA 城市 A 的編號
     * @param cityB 城市 B 的編號
     * @param distMatrix 預計算好的扁平化距離矩陣
     * @param n 城市總數
     * @return 兩城市間的歐幾里得距離
     */
    static double getDistance(int cityA, int cityB, const std::vector<double>& distMatrix, int n);

    /**
     * @brief 產生指定範圍內的隨機整數
     * @param min 最小值 (包含)
     * @param max 最大值 (包含)
     * @return 區間 $[min, max]$ 內的隨機整數
     */
    static int getRandomInt(int min, int max);

    /**
     * @brief 產生指定範圍內的隨機浮點數
     * @param min 最小值，預設為 0.0
     * @param max 最大值，預設為 1.0
     * @return 區間 $[min, max]$ 內的隨機浮點數
     */
    static double getRandomDouble(double min = 0.0, double max = 1.0);

    /**
     * @brief 獲取全域靜態隨機數引擎的引用
     * * 確保整個演化流程共用同一個實例，避免因短時間內重覆建立引擎導致隨機性失效。
     * @return std::mt19937 引擎引用
     */
    static std::mt19937& getGenerator() {
        return g_gen;
    }

private:
    /** * @brief 靜態隨機數引擎 (Mersenne Twister)
     * 具備極長的隨機週期，適合用於科學計算與演算法模擬。
     */
    static std::mt19937 g_gen;
};

#endif