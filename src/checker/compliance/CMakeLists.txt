project(compliance)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# 获取当前目录下所有目录并添加为子目录
file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
        add_subdirectory(${subdir})
    endif()
endforeach()
