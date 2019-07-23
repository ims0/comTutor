
include_directories(
    include
)

file(GLOB_RECURSE app_files
    src/*.cpp
)
add_library(${app_name} ${app_files})
