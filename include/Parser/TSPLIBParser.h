#ifndef TSPLIB_PARSER_H
#define TSPLIB_PARSER_H

#include <string>
#include <vector>
#include "Core/Types.h" // 使用自己定義的 City 結構

class TSPLIBParser {
public:
    /**
     * @brief 解析 TSPLIB 標準檔案 (.tsp)
     * @param filePath 檔案路徑
     * @return 轉換後的城市向量
     */
    static std::vector<City> parse(const std::string& filePath);
};

#endif