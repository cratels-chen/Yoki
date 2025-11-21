# generate_includes.cmake

message(STATUS "XXX run here??")
# 设置输入和输出路径
set(input_directory "${CMAKE_SOURCE_DIR}/src/checker/compliance")
message(STATUS "input_directory: ${input_directory}")
set(output_file "${CMAKE_BINARY_DIR}/compliance_common_include.h")
message(STATUS "output_file: ${output_file}")

# 打开输出文件进行写入
file(WRITE ${output_file} "// generated_includes.h - 自动生成的包含文件列表\n")

message(STATUS "ZZZZZ")
# 查找输入目录中的所有头文件（包括子目录）
file(GLOB_RECURSE header_files "${input_directory}/*.h")
message(STATUS "After GLOB_RECURSE")
message(STATUS "header_files: ${header_files}")

# 确保找到的头文件不为空
if(NOT header_files)
    message(WARNING "No header files found in ${input_directory}")
else()
    foreach(header IN LISTS header_files)
        # 获取相对路径，以便在生成的头文件中使用正确的相对路径
        file(RELATIVE_PATH relative_path ${CMAKE_SOURCE_DIR} ${header})
        file(APPEND ${output_file} "#include \"${relative_path}\"\n")
    endforeach()
endif()

# # 将每个头文件路径写入输出文件
# foreach(header IN LISTS header_files)
#     message(STATUS "Processing header file: ${header}")
#     # # 获取相对路径，以便在生成的头文件中使用正确的相对路径
#     # file(RELATIVE_PATH relative_path ${CMAKE_SOURCE_DIR} ${header})
#     # file(APPEND ${output_file} "#include \"${relative_path}\"\n")
# endforeach()
