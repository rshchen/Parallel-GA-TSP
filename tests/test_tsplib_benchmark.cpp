#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <map>
#include "Parser/TSPLIBParser.h"
#include "Core/GASolver.h"
#include "TestUtils.h"

struct Stats {
    double best;
    double mean;
    double stdDev;
    double gap;
    double avgTime;
};

// 輔助函式：計算標準差
double calculateStdDev(const std::vector<double>& data, double mean) {
    double sum = 0;
    for (double v : data) sum += (v - mean) * (v - mean);
    return std::sqrt(sum / data.size());
}

void runBenchmark(const std::string& name, const std::string& path, double optimalDist, int runs = 10) {
    std::cout << "\n>>> Benchmarking Instance: " << name << " (Optimal: " << optimalDist << ")" << std::endl;
    
    auto cities = TSPLIBParser::parse(path);
    
    
    GAConfig config;
    config.cityCount = cities.size();
    config.populationSize = 1500; // 針對大型問題加強
    config.generations = 3000;    // 增加收斂時間
    config.mutationRate = 0.15;   // 增加跳出局部解的機會
    config.tournamentSize = 3;    // 配合 2-Opt 的弱選擇壓力
    config.useParallel = true;

    // === 使用 TestUtils 優化後的 Callback 設定 ===
    // 直接呼叫工廠函式，產生每 100 代印點點的行為
    config.onGenerationComplete = GATestUtils::getDotsCallback(100);
    // ===========================================

    std::vector<double> results;
    std::vector<double> times;

    for (int i = 0; i < runs; ++i) {
        std::cout << "  > Progress: Run " << i + 1 << "/" << runs << " " << std::flush;
        
        GASolver solver(config, cities);
        
        auto start = std::chrono::high_resolution_clock::now();
        Individual res = solver.solve(); // solve 內部現在會觸發上面的 Lambda
        auto end = std::chrono::high_resolution_clock::now();
        
        double duration = std::chrono::duration<double>(end - start).count();
        results.push_back(res.distance);
        times.push_back(duration);
        
        std::cout << " Done! Best: " << std::fixed << std::setprecision(2) 
                  << res.distance << " (" << duration << "s)" << std::endl;
    }

    // 跑完後印出一個簡單的列表，方便分析
    std::cout << "\r  > All " << runs << " runs completed.                               " << std::endl;
    for(size_t i = 0; i < results.size(); ++i) {
        std::cout << "    Run " << std::setw(2) << i+1 << ": " << std::fixed << std::setprecision(2) << results[i] << std::endl;
    }

    double best = *std::min_element(results.begin(), results.end());
    double sum = std::accumulate(results.begin(), results.end(), 0.0);
    double mean = sum / runs;
    double stdDev = calculateStdDev(results, mean);
    double cv = (mean != 0) ? (stdDev / mean) * 100.0 : 0.0; // 計算 CV %
    double gap = ((best - optimalDist) / optimalDist) * 100.0;
    double avgTime = std::accumulate(times.begin(), times.end(), 0.0) / runs;

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "  [Result for " << name << "]" << std::endl;
    std::cout << "  Best Distance : " << std::fixed << std::setprecision(2) << best << std::endl;
    std::cout << "  Optimality Gap: " << gap << "%" << std::endl;
    std::cout << "  Mean Distance : " << mean << std::endl;
    std::cout << "  Std Deviation : " << stdDev << std::endl;
    std::cout << "  CV (Stability) : " << std::fixed << std::setprecision(2) << cv << "%" << std::endl; 
    std::cout << "  Avg Exec Time : " << avgTime << "s" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

int main() {
    // 定義測試目標與官方最優解
    std::map<std::string, double> targets = {
        {"berlin52", 7542.0},
        {"st70", 675.0},
        {"ch150", 6528.0}
    };

    std::cout << "===============================================" << std::endl;
    std::cout << "   TSPLIB Robustness & Performance Benchmark   " << std::endl;
    std::cout << "===============================================" << std::endl;

    for (auto const& [name, optimal] : targets) {
        std::string path = "../data/tsplib/" + name + ".tsp";
        try {
            runBenchmark(name, path, optimal, 10); 
        } catch (const std::exception& e) {
            std::cerr << "Error running " << name << ": " << e.what() << std::endl;
        }
    }

    return 0;
}