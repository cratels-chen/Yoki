project(Yoki VERSION 1.0.0)

# 优先使用 vcpkg 安装的包
# if(NOT DEFINED ENV{CMAKE_PREFIX_PATH})
#     message(FATAL_ERROR "CMAKE_PREFIX_PATH environment variable is not set.")
# endif()

list(APPEND CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/x64-linux")

# 查找 LLVM 和 Clang 包
find_package(LLVM REQUIRED CONFIG)

# 打印 LLVM 相关变量以进行调试
message(STATUS "Found LLVM: " ${LLVM_FOUND})
message(STATUS "LLVM version: " ${LLVM_VERSION})
message(STATUS "LLVM include directory: " ${LLVM_INCLUDE_DIRS})
message(STATUS "LLVM library directory: " ${LLVM_LIBRARY_DIRS})
message(STATUS "LLVM install path: " ${LLVM_INSTALL_PREFIX})

include(AddLLVM)
add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIRS})

# 显式添加库搜索路径和头文件搜索路径
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
include_directories(SYSTEM ${CMAKE_CURRENT_LIST_DIR}/third_party/llvm_18/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -w")

set (clang_paths
 ${LLVM_INSTALL_PREFIX}
 ${LLVM_INSTALL_PREFIX}/lib/cmake
 ${LLVM_INSTALL_PREFIX}/lib/cmake/llvm
 ${LLVM_INSTALL_PREFIX}/lib/cmake/clang
 ${LLVM_INSTALL_PREFIX}/share/clang/cmake
 ${LLVM_INSTALL_PREFIX}/share/llvm/cmake
)
find_package(Clang REQUIRED CONFIG PATHS "${clang_paths}")

find_package(spdlog REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  handler
  ast_frontend_action
  clangAnalysis
  clangAST
  clangBasic
  clangDriver
  clangEdit
  clangFrontend
  clangLex
  clangParse
  clangRewrite
  clangSema
  clangSerialization
  clangStaticAnalyzerCore
  clangStaticAnalyzerFrontend
  clangTooling
  defect
  config
  utils
)

add_subdirectory(ast_traverse)
add_subdirectory(checker_manager)
add_subdirectory(checker)
add_subdirectory(common)
add_subdirectory(config)
add_subdirectory(defect_manager)
add_subdirectory(defect)
add_subdirectory(handler)
add_subdirectory(ut_generator)
add_subdirectory(utils)
add_subdirectory(function_decl_node)
