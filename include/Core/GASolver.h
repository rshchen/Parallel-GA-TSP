#ifndef GASOLVER_H
#define GASOLVER_H

#include "Core/Types.h"
#include "Core/ParallelEvaluator.h"
#include <vector>

/**
 * @class GASolver
 * @brief 遺傳演算法求解器，專用於解決旅行推銷員問題 (TSP)
 * * 本類別採用 Memetic Algorithm 架構，結合了遺傳演算法 (GA) 的全域搜尋能力
 * 與 2-Opt 局部搜尋的開發能力。支援精英保留策略與多執行緒平行評估。
 */
class GASolver {
public:
    /**
     * @brief 建構子：初始化求解器設定與城市資料
     * @param config GA 的參數設定 (包含族群大小、代數、突變率等)
     * @param cities 城市座標列表，用於建立距離查表矩陣
     */
    GASolver(const GAConfig& config, const std::vector<City>& cities);

    /**
     * @brief 初始化族群
     * 隨機產生初始路徑序列，並執行初次的適應度評估。
     */
    void initPopulation();

    /**
     * @brief 執行主演化循環 (Main Evolution Loop)
     * * 流程包含：排序、精英保留、錦標賽選擇、順序交叉 (OX)、交換突變及族群更新。
     * 每一代演化後會透過 Callback 回報當前進度。
     * @return 返回演化過程中找到的最佳個體 (Individual)
     */
    Individual solve();

    /**
     * @brief 獲取當前族群中最優秀的個體
     * @return 當前代數中距離最短的個體副本
     */
    Individual getBestIndividual() const;

private:
    // --- 核心演化算子 (Internal Evolutionary Operators) ---
    
    /**
     * @brief 計算個體的總路徑距離
     * 透過預計算的距離矩陣進行 $O(1)$ 查表。
     * @param ind 待評估的個體
     */
    void evaluateIndividual(Individual& ind);
    
    /**
     * @brief 錦標賽選擇 (Tournament Selection)
     * 從族群中隨機抽選 $k$ 個個體，回傳其中表現最佳者，平衡選擇壓力與多樣性。
     * @return 被選中的精英個體
     */
    Individual selectionTournament();
    
    /**
     * @brief 順序交叉 (Order Crossover, OX)
     * 專為 TSP 設計的算子，在保留父代相對順序的同時保證路徑的合法性。
     * @param p1 父代個體 1
     * @param p2 父代個體 2
     * @return 交叉產生的子代個體
     */
    Individual crossoverOX(const Individual& p1, const Individual& p2);
    
    /**
     * @brief 交換突變 (Swap Mutation)
     * 隨機選取路徑中的兩個節點進行對調，以引入隨機擾動跳出局部最優。
     * @param ind 欲進行突變的個體
     */
    void mutate(Individual& ind);

    /**
     * @brief 局部搜尋優化 (2-Opt Local Search)
     * 針對個體路徑進行邊交換優化，消除交叉路徑，是提升精準度的關鍵算子。
     * @param ind 欲進行局部優化的個體
     */
    void apply2Opt(Individual& ind);

    // --- 私有成員變數 (Internal State) ---

    /** @brief 演算法參數配置 */
    GAConfig m_config;

    /** @brief 原始城市座標資料 */
    std::vector<City> m_cities;

    /** @brief 扁平化距離矩陣 ($N \times N$)，提升快取友善度 */
    std::vector<double> m_distMatrix;

    /** @brief 當前代數的族群集合 */
    std::vector<Individual> m_population;

    /** @brief 平行評估器，負責調度多執行緒計算資源 */
    ParallelEvaluator m_evaluator;
};

#endif