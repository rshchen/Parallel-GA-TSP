#include "Parser/TSPLIBParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

std::vector<City> TSPLIBParser::parse(const std::string& filePath) {
    std::vector<City> cities;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("TSPLIBParser: Cannot open file " + filePath);
    }

    std::string line;
    bool isCoordSection = false;

    while (std::getline(file, line)) {
        // 去除行首尾空格（簡單處理）
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        if (line.empty()) continue;

        // 偵測數據段落開始
        if (line.find("NODE_COORD_SECTION") != std::string::npos) {
            isCoordSection = true;
            continue;
        }

        // 偵測結束
        if (line.find("EOF") != std::string::npos) {
            break;
        }

        // 解析座標
        if (isCoordSection) {
            std::stringstream ss(line);
            int id;
            double x, y;
            if (ss >> id >> x >> y) {
                // TSPLIB ID 通常從 1 開始，我們系統轉為 0-based
                cities.push_back({id - 1, x, y});
            }
        }
    }

    if (cities.empty()) {
        throw std::runtime_error("TSPLIBParser: No coordinates found in " + filePath);
    }

    return cities;
}