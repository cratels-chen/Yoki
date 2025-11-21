project(compliance)

#  生成公共头文件
set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/src/checker/compliance)
set(PUBLIC_HEADER ${CMAKE_BINARY_DIR}/compliance_public_header.h)

file(GLOB_RECURSE HEADER_FILES ${HEADERS_DIR}/*.h)

file(WRITE ${PUBLIC_HEADER} "// This is an auto-generated header file\n")

foreach(HEADER ${HEADER_FILES})
    file(RELATIVE_PATH REL_PATH ${HEADERS_DIR} ${HEADER})
    file(APPEND ${PUBLIC_HEADER} "#include \"${REL_PATH}\"\n")
endforeach()


# 打印生成的公共头文件路径
message(STATUS "Generated public header: ${PUBLIC_HEADER}")

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_BINARY_DIR}
)

# 获取当前目录下所有目录并添加为子目录
file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
        add_subdirectory(${subdir})
    endif()
endforeach()
