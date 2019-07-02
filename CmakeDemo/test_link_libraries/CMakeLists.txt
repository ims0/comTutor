cmake_minimum_required(VERSION 3.0)

project(test_depend)
add_definitions("-Wall  -g")

AUX_SOURCE_DIRECTORY(src dir_srcs)

add_executable(${PROJECT_NAME} ${dir_srcs})

add_subdirectory(subA)
add_subdirectory(subB_dependA)

link_libraries(addFun printFun )
target_link_libraries(${PROJECT_NAME}  addFun printFun)

