project(checker_manager)

add_library(${PROJECT_NAME} CheckerManager.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
  checker_base
  checker_utils
  compliance
  defect
  misra_cpp2023
  spdlog::spdlog
)
