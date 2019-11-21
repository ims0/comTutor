#1.cmake verson，指定cmake版本
cmake_minimum_required(VERSION 3.0)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应

set(CMAKE_C_COMPILER  "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER  "/usr/bin/g++")

PROJECT(test_math)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}     -std=c99   -Wall -Wfatal-errors -O1 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wfatal-errors -O1")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}     -std=c99   -Wall -Wfatal-errors -g3 ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -Wall -Wfatal-errors -g3 ")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/output")

#### debug info #####
#变量引用时候，{变量名与大括号之间不能有空格}
message(CMAKE_C_COMPILER: ${CMAKE_C_COMPILER})
message(CMAKE_C_FLAGS:    ${CMAKE_C_FLAGS})
#Debug, Release, MinSizeRel, RelWithDebInfo
message(CMAKE_BUILD_TYPE:    ${CMAKE_BUILD_TYPE})
MESSAGE(STATUS $ENV{CC})
MESSAGE(STATUS $ENV{CXX})
MESSAGE(STATUS $ENV{HOME})
add_definitions("-Wall -lpthread -g")

#head file path
INCLUDE_DIRECTORIES(
include
)

#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

#set environment variable
SET(TEST_MATH  
${DIR_SRCS}
)

#set extern libraries
SET(LIBRARIES
libm.so
)

message(DIR_SRCS:${DIR_SRCS})
message(TEST_MATH:${TEST_MATH})
#add executable file
ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH})

#add link library
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)

