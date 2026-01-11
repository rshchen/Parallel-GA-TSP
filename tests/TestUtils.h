#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
#include <functional>

/**
 * @namespace GATestUtils
 * @brief 遺傳演算法測試輔助工具集
 * * 提供一組預定義的 Callback 工廠函式，用於在測試或標竿測試中快速注入
 * 演化進度顯示邏輯。
 */
namespace GATestUtils {

/**
 * @brief 產生一個簡約的進度回報回呼 (印點點模式)
 * * 適用於大規模標竿測試，在不佔用過多終端機空間的情況下提供執行回饋。
 * @param interval 每隔多少代 (Generation) 印出一個點，預設為 100
 * @return 符合 GAConfig 規格的 std::function 回呼函式
 */
inline std::function<void(int, double)> getDotsCallback(int interval = 100) {
    // 透過 Lambda 捕捉 interval 參數實現閉包 (Closure)
    return [interval](int gen, double /*bestDist*/) {
        if (gen % interval == 0) {
            std::cout << "." << std::flush;
        }
    };
}

/**
 * @brief 產生一個詳細的進度回報回呼 (收斂監控模式)
 * * 適合用於觀察演算法的收斂曲線，會印出代數與當前的最佳路徑距離。
 * @param interval 每隔多少代印出一次詳細資訊，預設為 500
 * @return 符合 GAConfig 規格的 std::function 回呼函式
 */
inline std::function<void(int, double)> getVerboseCallback(int interval = 500) {
    return [interval](int gen, double bestDist) {
        if (gen % interval == 0) {
            std::cout << "\n[Test Progress] Gen " << gen 
                      << " | Current Best: " << bestDist << std::flush;
        }
    };
}

} // namespace GATestUtils

#endif // TEST_UTILS_H