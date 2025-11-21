project(ast_consumer)

add_library(${PROJECT_NAME} YokiStaticScanASTConsumer.cpp YokiUTGenerationASTConsumer.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  ast_visitor
)
