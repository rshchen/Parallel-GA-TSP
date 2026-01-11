#include "Core/GASolver.h"
#include "Core/Utils.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>


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
        // 1. 產生 [0, 1, 2, ..., n-1] 的序列
        std::iota(ind.path.begin(), ind.path.end(), 0);
        // 2. 隨機打亂路徑
        std::shuffle(ind.path.begin(), ind.path.end(), Utils::getGenerator());
        
        // --- 這裡不再呼叫 evaluateIndividual(ind) ---
    }

    // 3. 【關鍵】初始化完畢後，統一進行第一次評估
    // 這樣可以保證進入 solve() 的第一個迴圈時，大家都有分數了
    m_evaluator.evaluate(m_population, m_distMatrix, m_config.cityCount, m_config.useParallel);
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



Individual GASolver::crossoverOX(const Individual& p1, const Individual& p2) {
    int n = m_config.cityCount;
    Individual child;
    child.path.assign(n, -1); // -1 標記為未填充
    std::vector<bool> visited(n, false); // O(1) 查表：記錄哪些城市已放入小孩路徑

    // 1. 隨機選取切點 (Cut Points)
    int start = Utils::getRandomInt(0, n - 2);
    int end = Utils::getRandomInt(start + 1, n - 1);

    // 2. 繼承親代 1 的中間片段 (Segment Inheritance)
    // 目的：保留親代 A 的局部優良路徑結構
    for (int i = start; i <= end; ++i) {
        child.path[i] = p1.path[i];
        visited[p1.path[i]] = true; // 標記已訪問，避免重複
    }

    // 3. 環狀填補 (Circular Filling)
    // 目的：按照親代 2 的順序填補剩餘位置，同時保護「環狀鄰接關係 (Circular Adjacency)」
    // 技巧：從切點後方開始填補，利用 % n 達成首尾相連的環狀搜尋
    int current_child_pos = (end + 1) % n; // 小孩放置基因的指針
    int p2_pos = (end + 1) % n;           // 媽媽讀取基因的指針

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

Individual GASolver::selectionTournament() {
    // 錦標賽規模，通常設定為族群大小的 5% ~ 10%
    // 如果 config 沒定義，我們預設為 5
    int k = m_config.tournamentSize; 
    
    // 先隨機選一個作為目前最強的基準
    int bestIdx = Utils::getRandomInt(0, m_population.size() - 1);
    Individual best = m_population[bestIdx];

    // 進行 k-1 次抽樣比較
    for (int i = 1; i < k; ++i) {
        int randIdx = Utils::getRandomInt(0, m_population.size() - 1);
        // 如果抽到更強的（距離更短），就更新最佳者
        if (m_population[randIdx].distance < best.distance) {
            best = m_population[randIdx];
        }
    }

    return best; 
}

Individual GASolver::solve() {
    // 1. 初始化族群並完成第一代評估
    initPopulation(); 

    // 初始化 bestEver 為第一代中的最強者
    // (因為 initPopulation 最後已經呼叫過 evaluate，所以這裡可以安全排序)
    std::sort(m_population.begin(), m_population.end());
    Individual bestEver = m_population[0];

    for (int gen = 0; gen < m_config.generations; ++gen) {
        
        // --- A. 產生下一代 ---
        std::vector<Individual> nextPopulation;
        nextPopulation.reserve(m_config.populationSize);

        // 精英保留 (5%)
        int elitismCount = std::max(1, (int)(m_config.populationSize * 0.05));
        for (int i = 0; i < elitismCount; ++i) {
            nextPopulation.push_back(m_population[i]);
        }

        // 繁衍 (Crossover & Mutation)
        while (nextPopulation.size() < m_config.populationSize) {
            Individual p1 = selectionTournament();
            Individual p2 = selectionTournament();
            Individual child = crossoverOX(p1, p2);
            mutate(child);
            nextPopulation.push_back(child);
        }

        // --- B. 族群更迭 ---
        m_population = std::move(nextPopulation);

        // --- C. 統一平行評估 ---
        // 這裡負責計算這一代所有新小孩的距離
        m_evaluator.evaluate(m_population, m_distMatrix, m_config.cityCount, m_config.useParallel);

        // --- D. 排序與記錄 ---
        std::sort(m_population.begin(), m_population.end());
        if (m_population[0].distance < bestEver.distance) {
            bestEver = m_population[0];
            // 建議加一個代數標記，方便觀察收斂速度
            std::cout << "[Gen " << gen << "] New Best Distance: " << bestEver.distance << std::endl;
        }
    }

    return bestEver;
}


Individual GASolver::getBestIndividual() const {
    if (m_population.empty()) {
        // 如果族群還是空的（還沒 init），回傳一個空的 Individual
        return Individual();
    }
    // 傳回目前排序第一名的個體
    return m_population[0];
}