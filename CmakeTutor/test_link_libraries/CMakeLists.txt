cmake_minimum_required(VERSION 3.0)
#main cmake
project(${PROJECT_NAME})
message(STATUS project:${PROJECT_NAME})
message(STATUS PROJECT_SOURCR_DIR:${PROJECT_SOURCR_DIR})
add_definitions("-Wall  -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include_directories(
    "code/include"
    )
add_subdirectory(sub_lib_a)
add_subdirectory(sub_lib_b)

AUX_SOURCE_DIRECTORY(code/src dir_srcs)
message(STATUS src:${dir_srcs})

add_executable(${PROJECT_NAME} ${dir_srcs})
link_libraries( print_lib add)


#依赖库的顺序，被依赖的库放在最后
target_link_libraries(${PROJECT_NAME} print_lib add )

#error order
#target_link_libraries(${PROJECT_NAME} add print_lib )

