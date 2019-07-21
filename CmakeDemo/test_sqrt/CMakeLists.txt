#1.cmake verson，指定cmake版本
cmake_minimum_required(VERSION 3.0)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(test_math)

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

message(DIR_SRCS:$(DIR_SRCS))
message(TEST_MATH:$(TEST_MATH))
#add executable file
ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH})

#add link library
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)

