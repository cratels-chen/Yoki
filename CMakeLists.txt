cmake_minimum_required(VERSION 3.20)

# 查找 clang 和 clang++ 编译器
find_program(CLANG_COMPILER NAMES clang)
find_program(CLANGXX_COMPILER NAMES clang++)
message(STATUS "Clang compiler: ${CLANG_COMPILER}")
message(STATUS "Clang++ compiler: ${CLANGXX_COMPILER}")

# 若当前环境未安装 clang 编译器则直接报错
if(NOT CLANG_COMPILER)
    message(FATAL_ERROR "Clang compiler not found. Please install Clang.")
endif()

if(NOT CLANGXX_COMPILER)
    message(FATAL_ERROR "Clang++ compiler not found. Please install Clang++.")
endif()

# 设置编译器的版本必须在project命令之前
# 设置 C 和 C++ 编译器为 clang 和 clang++
set(CMAKE_C_COMPILER ${CLANG_COMPILER})
set(CMAKE_CXX_COMPILER ${CLANGXX_COMPILER})

# 设置默认构建类型为 Release，如果未指定的话
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

project(Yoki VERSION 1.0.0)

# 设置当前项目 C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# 启用生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 检查 VCPKG_ROOT 是否设置
if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not set. Please set it to the path where vcpkg is installed.")
else()
    set(VCPKG_ROOT $ENV{VCPKG_ROOT})
endif()

# 配置 CMake 使用 Vcpkg 的 toolchain 查找 Vcpkg 依赖包
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

add_subdirectory(src)

# 复制资源文件到构建目录
configure_file(${CMAKE_SOURCE_DIR}/resources/yoki_logo.png 
               ${CMAKE_BINARY_DIR}/resources/yoki_logo.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources/Yoki_framework.png 
               ${CMAKE_BINARY_DIR}/resources/Yoki_framework.png COPYONLY)
