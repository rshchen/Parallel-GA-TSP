cmake_minimum_required(VERSION 3.15)
project(Parallel-GA-TSP VERSION 1.0.0 LANGUAGES CXX)

# 設定 C++ 標準
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 尋找多執行緒套件 (pthread)
find_package(Threads REQUIRED)

# 包含路徑：現在標頭檔在 include/Core 和 include/Parser 下
# 使用 include_directories(include) 代表程式碼內需寫 #include "Core/GASolver.h"
include_directories(include)

# 注入絕對路徑定義 (解決路徑問題的專業解法)
add_definitions(-DTSPLIB_DATA_DIR="${CMAKE_SOURCE_DIR}/data/tsplib/")

# 1. 建立核心演算法與解析器靜態庫
# 更新路徑至 src/Core 和 src/Parser
add_library(ga_solver_lib STATIC 
    src/Core/Utils.cpp
    src/Core/GASolver.cpp
    src/Core/ParallelEvaluator.cpp
    src/Parser/TSPLIBParser.cpp
)

# 2. 編譯主程式 (Example)
add_executable(tsp_solver examples/main.cpp)
target_link_libraries(tsp_solver PRIVATE ga_solver_lib Threads::Threads)

# 3. 編譯單元測試 (Unit Tests)
add_executable(test_utils tests/test_utils.cpp)
target_link_libraries(test_utils PRIVATE ga_solver_lib Threads::Threads)

add_executable(test_ga tests/test_ga.cpp)
target_link_libraries(test_ga PRIVATE ga_solver_lib Threads::Threads)

add_executable(test_parallel tests/test_parallel.cpp)
target_link_libraries(test_parallel PRIVATE ga_solver_lib Threads::Threads)

add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE ga_solver_lib Threads::Threads)

add_executable(test_tsplib_benchmark tests/test_tsplib_benchmark.cpp)
target_link_libraries(test_tsplib_benchmark PRIVATE ga_solver_lib Threads::Threads)

target_include_directories(test_tsplib_benchmark PRIVATE tests)



# 設定編譯優化 (Release 模式下開啟 -O3)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    target_compile_options(ga_solver_lib PRIVATE /O2 /W4)
else()
    target_compile_options(ga_solver_lib PRIVATE -O3 -Wall -Wextra)
endif()