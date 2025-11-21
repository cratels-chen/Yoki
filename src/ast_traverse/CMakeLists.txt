project(ast_traverse)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(ast_consumer)
add_subdirectory(ast_frontend_action)
add_subdirectory(ast_visitor)
