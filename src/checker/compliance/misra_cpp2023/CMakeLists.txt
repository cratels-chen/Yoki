project(misra_cpp2023)

# 获取当前目录下的所有子目录
file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

# 遍历每个子目录
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
        add_subdirectory(${SUBDIR})

        # 将子目录名作为库名添加到 TARGET_LIBS 列表中
        list(APPEND TARGET_LIBS ${SUBDIR})
    endif()
endforeach()

# 创建一个 INTERFACE 库
add_library(${PROJECT_NAME} INTERFACE)

# 链接所有子目录生成的库
target_link_libraries(${PROJECT_NAME} INTERFACE ${TARGET_LIBS})
