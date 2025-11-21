project(checker_base)

add_library(${PROJECT_NAME} CheckerBase.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  common
  checker_utils
)
