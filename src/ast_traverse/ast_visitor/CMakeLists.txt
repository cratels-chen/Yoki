project(ast_visitor)

add_library(${PROJECT_NAME} YokiStaticScanASTVisitor.cpp YokiUTGenerationASTVisitor.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  common
  checker_manager
)
