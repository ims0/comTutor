cmake_minimum_required(VERSION 3.0)
#main cmake
project(${PROJECT_NAME})
message(STATUS PROJECT_NAME:${PROJECT_NAME})

message(STATUS PROJECT_SOURCR_DIR:${PROJECT_SOURCR_DIR})
message(STATUS CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR})

add_definitions("-Wall  -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories( "./")
include_directories("code/inc/")
add_subdirectory(add_lib)
add_subdirectory(print_lib)

AUX_SOURCE_DIRECTORY(code/src dir_srcs)
message(STATUS src:${dir_srcs})

add_executable(${PROJECT_NAME} ${dir_srcs})

link_libraries( print_lib add_lib)

set(HELLO "hello")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/code/inc/internal.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/code/inc/internal.h")

#依赖库的顺序，被依赖的库放在最后
target_link_libraries(${PROJECT_NAME} print_lib add_lib )

#error order
#target_link_libraries(${PROJECT_NAME} add print_lib )

