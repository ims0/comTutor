cmake_minimum_required(VERSION 3.0)
message(printFun)

AUX_SOURCE_DIRECTORY(src subdir_srcs)
add_library(printFun ${subdir_srcs})
