project(defect_manager)

add_library(${PROJECT_NAME} DefectManager.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  defect
  config
)
