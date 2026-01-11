#include <iostream>
#include <cassert>
#include <vector>
#include "Parser/TSPLIBParser.h"
#include "Core/Types.h"

/**
 * [ 測試目的：TSPLIB 標準格式解析驗證 ]
 * 1. 檔案讀取驗證：確保 Parser 能正確定位並打開相對路徑下的 .tsp 檔案。
 * 2. 格式解析正確性：
 * - 驗證是否能正確識別 'NODE_COORD_SECTION' 標籤並開始解析。
 * - 驗證 ID 是否正確從 1-based (TSPLIB 規範) 轉換為系統內部的 0-based。
 * 3. 資料完整性 (Data Integrity)：
 * - 驗證城市總數 (Dimension) 是否與官方規格一致。
 * - 驗證首筆關鍵座標 (First Entry) 的數值準確度，防止浮點數轉換誤差。
 */

int main() {
    std::cout << "--- Testing TSPLIB Parser: Integration & Integrity ---" << std::endl;

    try {
        // 配置測試路徑 (相對路徑需依據執行檔執行位置調整)
        std::string path = "../data/tsplib/berlin52.tsp";
        
        // 執行解析
        auto cities = TSPLIBParser::parse(path);

        // 驗證 1: 城市數量 (berlin52 規格應為 52 個城市)
        std::cout << "[Check 1] Dimension Verification: ";
        if (cities.size() == 52) {
            std::cout << "SUCCESS (" << cities.size() << ")" << std::endl;
        } else {
            throw std::runtime_error("Dimension mismatch! Expected 52, got " + std::to_string(cities.size()));
        }

        // 驗證 2: 座標轉換精度與 ID 位移
        // 官方數據第一筆: 1 565.0 575.0 -> 系統應存為 ID:0, X:565.0, Y:575.0
        std::cout << "[Check 2] Data Integrity (First Node): ";
        assert(cities[0].id == 0);
        assert(cities[0].x == 565.0);
        assert(cities[0].y == 575.0);
        std::cout << "SUCCESS (ID:0, X:565, Y:575)" << std::endl;

        // 驗證 3: 邊界數據 (最後一筆座標)
        // 官方數據最後一筆 (52): 52 1740.0 245.0
        std::cout << "[Check 3] Data Integrity (Last Node): ";
        assert(cities.back().id == 51);
        assert(cities.back().x == 1740.0);
        assert(cities.back().y == 245.0);
        std::cout << "SUCCESS (ID:51, X:1740, Y:245)" << std::endl;

        std::cout << "\n[RESULT] All TSPLIB Parser tests PASSED." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "\n[RESULT] TSPLIB Parser test FAILED: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}