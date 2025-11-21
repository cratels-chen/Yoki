project(function_decl_node)

find_package(spdlog REQUIRED)

add_library(${PROJECT_NAME} FunctionDeclNode.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  spdlog::spdlog
)
