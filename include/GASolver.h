#ifndef GASOLVER_H
#define GASOLVER_H

#include "Types.h"
#include <vector>

class GASolver {
public:
    /**
     * @brief 建構子
     * @param config GA 的參數設定 (P, G, mutationRate 等)
     * @param cities 城市座標資料
     */
    GASolver(const GAConfig& config, const std::vector<City>& cities);

    /**
     * @brief 初始化族群並計算初始適應度
     */
    void initPopulation();

    /**
     * @brief 執行演化迴圈
     * @return 返回最終找到的最佳個體 (路徑解)
     */
    Individual solve();

    /**
     * @brief 獲取當前族群中最優秀的個體
     */
    Individual getBestIndividual() const;

private:
    // --- 核心演化算子 ---
    
    // 計算單個個體的總距離與適應度
    void evaluateIndividual(Individual& ind);
    
    // 錦標賽選擇：選出較優秀的親代
    Individual selectionTournament();
    
    // 順序交叉 (Order Crossover)
    Individual crossoverOX(const Individual& p1, const Individual& p2);
    
    // 交換突變 (Swap Mutation)
    void mutate(Individual& ind);

    // --- 資料成員 ---
    GAConfig m_config;
    std::vector<City> m_cities;
    std::vector<double> m_distMatrix; // 扁平化距離矩陣 (O(1) 查表)
    std::vector<Individual> m_population;
};

#endif