#ifndef GASOLVER_H
#define GASOLVER_H

#include "Types.h"
#include "ParallelEvaluator.h"
#include <vector>

/**
 * @class GASolver
 * @brief 遺傳演算法求解器，專用於解決旅行推銷員問題 (TSP)
 * * 該類別封裝了族群管理、演化算子（選擇、交叉、突變）以及主演化循環。
 * 支援精英保留策略與 $O(1)$ 距離查表優化。
 */
class GASolver {
public:
    /**
     * @brief 建構子：初始化求解器設定與城市資料
     * @param config GA 的參數設定 (包含族群大小、代數、突變率等)
     * @param cities 城市座標列表，用於計算距離矩陣
     */
    GASolver(const GAConfig& config, const std::vector<City>& cities);

    /**
     * @brief 初始化族群
     * 產生隨機路徑序列並計算初始適應度。
     */
    void initPopulation();

    /**
     * @brief 執行主演化循環 (Main Evolution Loop)
     * 包含排序、精英保留、選擇、交叉、突變及族群更新。
     * @return 返回演化過程中找到的最佳個體 (Individual)
     */
    Individual solve();

    /**
     * @brief 獲取當前族群中最優秀的個體
     * @return 當前代數中距離最短的個體副本
     */
    Individual getBestIndividual() const;

private:
    // --- 核心演化算子 (內部邏輯) ---
    
    /**
     * @brief 計算個體的總路徑距離與適應度
     * @param ind 待評估的個體
     */
    void evaluateIndividual(Individual& ind);
    
    /**
     * @brief 錦標賽選擇 (Tournament Selection)
     * 從族群中隨機抽選 $k$ 個個體，回傳其中表現最佳者。
     */
    Individual selectionTournament();
    
    /**
     * @brief 順序交叉 (Order Crossover, OX)
     * 專為 TSP 設計的交叉算子，保證路徑合法性並保護相對順序。
     */
    Individual crossoverOX(const Individual& p1, const Individual& p2);
    
    /**
     * @brief 交換突變 (Swap Mutation)
     * 隨機選取路徑中的兩個點進行對調，以增加基因多樣性。
     */
    void mutate(Individual& ind);

    // --- 私有成員變數 (內部狀態) ---

    /** @brief 演算法參數配置 (唯讀) */
    GAConfig m_config;

    /** @brief 原始城市座標資料 */
    std::vector<City> m_cities;

    /** @brief 預計算的扁平化距離矩陣，提供 $O(1)$ 查表效能 */
    std::vector<double> m_distMatrix;

    /** @brief 當前代數的族群集合 */
    std::vector<Individual> m_population;

    ParallelEvaluator m_evaluator;
};

#endif