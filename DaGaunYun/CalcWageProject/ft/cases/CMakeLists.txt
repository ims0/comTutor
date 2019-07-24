
cmake_minimum_required(VERSION 3.0)
set(curr_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(app_name app)
set(app_dir  ${curr_dir}/../../code/${app_name})
set(tools_dir ${curr_dir}/../../tools)
set(ft_name ft)

project($(ft_name))

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -Wall -Wfatal-errors -ggdb -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -g3 -Wall -Wfatal-errors -ggdb -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -g3 -Wall -Wfatal-errors -ggdb -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -Wall -Wfatal-errors -fprofile-arcs -ftest-coverage")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../output")
include_directories(
    ${app_dir}/include
    ${curr_dir}/include
)

file(GLOB_RECURSE files
    ${curr_dir}/src/*.cpp
)

add_subdirectory(${app_dir} ${app_name})

IF(TOOLS)
    message(STATUS "Lib from defined tools in cmake.")
    include_directories(
        ${ tools_dir}/3rd/test/include)
    link_directories("${tools_dir}/3rd/test/lib")
ELSE()
    message(STATUS "Lib from install tools in cmake.")
ENDIF()

add_executable(${ft_name} ${files})
target_link_libraries(${ft_name} ${app_name} gtest pthread )
