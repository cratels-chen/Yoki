cmake_minimum_required(VERSION 3.20)

project(no_goto)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 查找并配置GTest
find_package(GTest REQUIRED)
include(GoogleTest)

# 如果系统没有安装GTest，使用FetchContent下载
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50f33f9a5c1e2b9b36f9ee73b5c8b.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# 创建库文件，包含f3函数
add_library(f3_lib f3_function.cpp)
target_include_directories(f3_lib PUBLIC .)

# 原始的可执行文件
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} f3_lib)

# 测试可执行文件
add_executable(test_f3 test_f3.cpp)
target_link_libraries(test_f3 f3_lib GTest::gtest GTest::gtest_main)

# 添加测试到CTest
gtest_discover_tests(test_f3)

