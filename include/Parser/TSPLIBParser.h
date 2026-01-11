#ifndef TSPLIB_PARSER_H
#define TSPLIB_PARSER_H

#include <string>
#include <vector>
#include "Core/Types.h"

/**
 * @class TSPLIBParser
 * @brief TSPLIB 標準檔案解析器
 * * 本類別負責解析旅行推銷員問題的標準基準測試檔案 (.tsp)。
 * 專注於解析 `NODE_COORD_SECTION` 區段，將原始文本資料轉換為系統內部的 City 結構向量。
 */
class TSPLIBParser {
public:
    /**
     * @brief 解析 TSPLIB 標準檔案 (.tsp)
     * * 讀取並解析給定路徑的檔案。該函式會自動跳過 metadata 標頭（如 NAME, TYPE, DIMENSION 等），
     * 並精確提取各城市的 ID 與二維座標資訊。
     * * @param filePath TSPLIB 格式檔案的完整路徑
     * @return 轉換後的城市資料向量 (std::vector<City>)
     * @throw std::runtime_cast 若檔案無法開啟或格式不符合標準時可能拋出異常
     */
    static std::vector<City> parse(const std::string& filePath);
};

#endif