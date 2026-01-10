cmake_minimum_required(VERSION 3.15)
project(Parallel-GA-TSP VERSION 1.0.0 LANGUAGES CXX)

# 設定 C++ 標準
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 尋找多執行緒套件 (pthread)
find_package(Threads REQUIRED)

# 包含路徑
include_directories(include)

# 1. 建立核心演算法靜態庫
# 將所有的實作檔封裝在一起，方便給 examples 和 tests 使用
add_library(ga_solver_lib STATIC 
    src/Utils.cpp
    src/GASolver.cpp
    src/ParallelEvaluator.cpp
)

# 2. 編譯主程式 (Example)
add_executable(tsp_solver examples/main.cpp)
target_link_libraries(tsp_solver PRIVATE ga_solver_lib Threads::Threads)

# 3. 編譯單元測試 (Unit Tests)
# 每個測試都是一個獨立的執行檔
add_executable(test_utils tests/test_utils.cpp)
target_link_libraries(test_utils PRIVATE ga_solver_lib Threads::Threads)

add_executable(test_ga tests/test_ga.cpp)
target_link_libraries(test_ga PRIVATE ga_solver_lib Threads::Threads)

add_executable(test_parallel tests/test_parallel.cpp)
target_link_libraries(test_parallel PRIVATE ga_solver_lib Threads::Threads)

# 設定編譯優化 (GA 非常需要 -O3)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(ga_solver_lib PRIVATE /O2)
    else()
        target_compile_options(ga_solver_lib PRIVATE -O3)
    endif()
endif()

