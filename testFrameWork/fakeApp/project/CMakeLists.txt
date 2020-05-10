cmake_minimum_required(VERSION 3.0)
message(STATUS ${CMAKE_BUILD_TYPE})
set(curr_dir ${CMAKE_CURRENT_SOURCE_DIR})

project(${PROJECT_NAME})
message(STATUS ${PROJECT_NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -std=c99 -Wall -Wfatal-errors ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wfatal-errors ")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS}  -g3 ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 ")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -Wall -Wfatal-errors ")

set(ENABLE_COVERAGE ${COVERAGE_FLAG})
if(ENABLE_COVERAGE)
    message(STATUS "Use gcov for code coverage test")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()



set(code_path ${PROJECT_PATH}/../code)
message(STATUS ${code_path})
include_directories(
    ${PROJECT_PATH}/../code/include
    ${PROJECT_PATH}/../../infra/include
)

file(GLOB_RECURSE files
    ${PROJECT_PATH}/../code/*.c*
    ${PROJECT_PATH}/../../infra/*.c*
)

add_executable(${PROJECT_NAME} ${files})
target_link_libraries(${PROJECT_NAME} gtest pthread )
