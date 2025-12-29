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

# 静态自注册依赖全局静态对象的构造函数执行。
# 在 macOS 上，链接器可能会丢弃未被引用的静态库对象文件，导致注册不生效。
# 这里对每个规则库使用 -force_load，确保其对象文件被强制载入。
if(APPLE)
    foreach(lib IN LISTS TARGET_LIBS)
        target_link_options(${PROJECT_NAME} INTERFACE "-Wl,-force_load,$<TARGET_FILE:${lib}>")
    endforeach()
endif()
