project(defect)

add_library(${PROJECT_NAME} Defect.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  config
  checker_base
)
