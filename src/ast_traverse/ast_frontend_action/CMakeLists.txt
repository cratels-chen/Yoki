project(ast_frontend_action)

add_library(${PROJECT_NAME} YokiASTFrontendAction.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  ast_consumer
  config
)
