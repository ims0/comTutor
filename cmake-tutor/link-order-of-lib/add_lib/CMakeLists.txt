cmake_minimum_required(VERSION 3.0)


message(STATUS CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR})
AUX_SOURCE_DIRECTORY(src SrcFile)

message(STATUS ${SrcFile})
add_library(add_lib ${SrcFile})
