#1.cmake verson，指定cmake版本
cmake_minimum_required(VERSION 3.0)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(templete)

add_definitions("-w -lpthread -g")

#head file path
INCLUDE_DIRECTORIES(
include
)

#source directory
AUX_SOURCE_DIRECTORY( ./ DIR_SRCS)

#set environment variable
SET(TEST_MATH  
${DIR_SRCS}
)

#add executable file
ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH})

