project(MISRA_CPP2023_Rule_9_6_1)

add_library(${PROJECT_NAME} MISRA_CPP2023_Rule_9_6_1.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  checker_utils
  defect
  defect_manager
)
