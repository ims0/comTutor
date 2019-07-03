cmake_minimum_required(VERSION 3.0)

project(test_depend)
add_definitions("-Wall  -g")
set( root_dir "./")
set( inc_dir  "${root_dir}include")
set( test_dir  "${inc_dir}/include")

message(hello var root_dir: ${root_dir})
AUX_SOURCE_DIRECTORY(src dir_srcs)
add_executable(${PROJECT_NAME} ${dir_srcs})
include_directories(
    ${inc_dir} 
    ${root_dir}testinc/include
    )
add_subdirectory(subA)
add_subdirectory(subB_dependA)

link_libraries(addFun printFun )
target_link_libraries(${PROJECT_NAME}   printFun addFun)

