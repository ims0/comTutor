cmake_minimum_required(VERSION 3.0)
message(addFun)

AUX_SOURCE_DIRECTORY(src subdir_srcs)
add_library(addFun ${subdir_srcs})
