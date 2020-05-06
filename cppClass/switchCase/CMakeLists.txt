#1.cmake verson，指定cmake版本 
cmake_minimum_required(VERSION 3.0)

#2.指定项目的名称，一般和项目的文件夹名称对应
project(plane)

#3 编译选项
add_definitions("-Wall -g")

#4.head file path，头文件目录
INCLUDE_DIRECTORIES(
include
)

#5.source directory，源文件目录
AUX_SOURCE_DIRECTORY(
src DIR_SRCS
)

#6,变量赋值
set(src_file
${DIR_SRCS}
)

#7.add executable file，添加要编译的可执行文件
add_executable(${PROJECT_NAME} ${src_file})

#8.add link library
TARGET_LINK_LIBRARIES(${PROJECT_NAME} gtest_main gtest  pthread)

