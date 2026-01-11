#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <iostream>
#include <functional>

namespace GATestUtils {

/**
 * @brief 產生一個簡單的「印點點」進度回報回呼函式
 * * @param interval 每隔多少代 (Generation) 印出一個點，預設為 100
 * @return std::function<void(int, double)> 符合 GAConfig 規格的 Callback
 */
inline std::function<void(int, double)> getDotsCallback(int interval = 100) {
    // 透過 Lambda 捕捉 interval 參數
    return [interval](int gen, double /*bestDist*/) {
        if (gen % interval == 0) {
            std::cout << "." << std::flush;
        }
    };
}

/**
 * @brief (選配) 產生一個會顯示目前距離的進度回報器
 * 適合用在需要觀察收斂曲線的測試
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