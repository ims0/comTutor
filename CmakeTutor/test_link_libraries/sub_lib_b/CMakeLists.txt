cmake_minimum_required(VERSION 3.0)


message(STATUS CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR})
AUX_SOURCE_DIRECTORY(src1 SrcFile)
AUX_SOURCE_DIRECTORY(src2 SrcFile)

message(STATUS sub_lib_b/src:${SrcFile})
add_library(print_lib ${SrcFile})
