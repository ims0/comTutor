cmake_minimum_required(VERSION 3.0)

project(calcWage)
message(CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/output")
#add compile args
add_definitions("-g")
#add macro
add_definitions(-DENABLED)

include_directories(../app/include)

#source directory
#aux_sourcr_directory(<dir> <variable>)
aux_source_directory(../app/src SRCS_DIR)
aux_source_directory(./ SRCS_DIR)
message(SRCS_DIR:${SRCS_DIR})

#add executable file
add_executable(${PROJECT_NAME} ${SRCS_DIR})

