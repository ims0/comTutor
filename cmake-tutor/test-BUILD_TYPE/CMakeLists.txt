cmake_minimum_required(VERSION 3.0)
message(STATUS "CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_CONFIGURATION_TYPES:" ${CMAKE_CONFIGURATION_TYPES})
message(STATUS "OUTPUT_DIR:" ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_DIR}")
set(curr_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(app_name app)
set(tools_dir ${curr_dir}/../../tools)
set(ft_name ft)

#specify compiler
#注意cmake语法，"引号中的路径两边不能有空格"
SET(CMAKE_C_COMPILER   "/usr/bin/gcc" )
#SET(CMAKE_CXX_COMPILER "/usr/bin/clang++" )
project($(ft_name))

set(CMAKE_C_FLAGS            " -Wall -Wfatal-errors -std=c99 -g3   " )
set(CMAKE_C_FLAGS_DEBUG      "                                     " )
#set(CMAKE_C_FLAGS_RELEASE   "                                     " )
set(CMAKE_CXX_FLAGS          " -Wall -Wfatal-errors -std=c++11 -g3 " )
set(CMAKE_CXX_FLAGS_DEBUG    "                                     " )
#set(CMAKE_CXX_FLAGS_RELEASE "                                     " )

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -Wall -Wfatal-errors ")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
include_directories(
    ${curr_dir}/code/include
)
file(GLOB_RECURSE srcFiles
    ${curr_dir}/code/src/*.c*
)

if(TOOLS)
    message(STATUS "Lib from defined tools in cmake.")
    include_directories(
        ${ tools_dir}/3rd/test/include)
    link_directories("${tools_dir}/3rd/test/lib")
else()
    message(STATUS "Lib from install tools in cmake.")
endif()
add_executable(${ft_name} ${srcFiles})
