cmake_minimum_required(VERSION 3.0)

project(calcWage)
#debug info
message(CMAKE_CURRENT_SOURCE_DIR:$(CMAKE_CURRENT_SOURCE_DIR))
#add compile args
add_definitions("-g")
#add macro
add_definitions(-DENABLED)

include_directories(../app/include)

#source directory
#aux_sourcr_directory(<dir> <variable>)
aux_source_directory(../app/src DIR_SRCS)

#set environment variable
set(TEST_MATH  ${DIR_SRCS})

#add executable file
add_executable(${PROJECT_NAME} ${DIR_SRCS})

