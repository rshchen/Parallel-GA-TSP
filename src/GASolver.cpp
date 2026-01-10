#include "GASolver.h"
#include "Utils.h"
#include <algorithm>
#include <numeric>
#include <cmath>


// 把族群建立起來，並利用查表來計算路徑長度。
GASolver::GASolver(const GAConfig& config, const std::vector<City>& cities)
    : m_config(config), m_cities(cities) {
    // 預計算距離矩陣，存入 m_distMatrix
    m_distMatrix = Utils::precomputeDistanceMatrix(m_cities);
}

void GASolver::initPopulation() {
    m_population.resize(m_config.populationSize);
    int n = m_config.cityCount;

    for (auto& ind : m_population) {
        ind.path.resize(n);
        // 產生 [0, 1, 2, ..., n-1] 的序列
        std::iota(ind.path.begin(), ind.path.end(), 0);
        // 隨機打亂路徑
        std::shuffle(ind.path.begin(), ind.path.end(), Utils::getGenerator());
        // 初始計算適應度
        evaluateIndividual(ind);
    }
}

void GASolver::evaluateIndividual(Individual& ind) {
    double totalDist = 0.0;
    int n = m_config.cityCount;

    for (int i = 0; i < n; ++i) {
        int from = ind.path[i];
        int to = ind.path[(i + 1) % n]; // 回到起點形成封閉迴路
        totalDist += Utils::getDistance(from, to, m_distMatrix, n);
    }

    ind.distance = totalDist;
    ind.fitness = 1.0 / totalDist; // 距離越短，適應度越高
}


// 這是解決 TSP 問題的精華。OX 交叉 能夠保留親代的相對順序，而不會產生重複城市；交換突變 則能提供隨機探索的動力。
Individual GASolver::crossoverOX(const Individual& p1, const Individual& p2) {
    int n = m_config.cityCount;
    Individual child;
    child.path.assign(n, -1);
    std::vector<bool> visited(n, false);

    // 1. 隨機選取切點
    int start = Utils::getRandomInt(0, n - 2);
    int end = Utils::getRandomInt(start + 1, n - 1);

    // 2. 繼承親代 1 的中間片段
    for (int i = start; i <= end; ++i) {
        child.path[i] = p1.path[i];
        visited[p1.path[i]] = true;
    }

    // 3. 從親代 2 剩餘的城市中按順序填補小孩的其他空位
    int current_child_pos = (end + 1) % n;
    int p2_pos = (end + 1) % n;

    for (int i = 0; i < n; ++i) {
        int city_from_p2 = p2.path[p2_pos];
        if (!visited[city_from_p2]) {
            child.path[current_child_pos] = city_from_p2;
            current_child_pos = (current_child_pos + 1) % n;
        }
        p2_pos = (p2_pos + 1) % n;
    }
    return child;
}

void GASolver::mutate(Individual& ind) {
    // 使用 Swap Mutation: 隨機選兩個點交換
    if (Utils::getRandomDouble() < m_config.mutationRate) {
        int idx1 = Utils::getRandomInt(0, m_config.cityCount - 1);
        int idx2 = Utils::getRandomInt(0, m_config.cityCount - 1);
        std::swap(ind.path[idx1], ind.path[idx2]);
        evaluateIndividual(ind); // 突變後需重新評估
    }
}