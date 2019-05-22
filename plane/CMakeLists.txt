#1.cmake verson，指定cmake版本 
cmake_minimum_required(VERSION 3.0)

#2.指定项目的名称，一般和项目的文件夹名称对应
PROJECT(plane)

#3 编译选项
add_definitions("-Wall -lpthread -g")

#4.head file path，头文件目录
INCLUDE_DIRECTORIES(
include
)

#5.source directory，源文件目录
AUX_SOURCE_DIRECTORY(src_fun DIR_SRCS)

#6,变量赋值
SET(TEST_MATH
${DIR_SRCS}
)

#7.add executable file，添加要编译的可执行文件
add_executable(${PROJECT_NAME} ${DIR_SRCS})

#8.add link library，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
TARGET_LINK_LIBRARIES(${PROJECT_NAME} gtest_main gtest  pthread)

