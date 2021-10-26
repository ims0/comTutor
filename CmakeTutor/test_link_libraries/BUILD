cc_library(
    name = "add_lib",
    hdrs = [
            "add_lib/inc/add.h",
            "print_lib/inc/inc.h",
            ],
    includes=[ "print_lib/inc"],
    #strip_include_prefix = "add_lib",
    srcs = ["add_lib/src/addFun.cpp"],
    #copts = [ #"-Iprint_lib/inc" ],
)

cc_library(
    name = "print_lib",
    hdrs = ["print_lib/inc/inc.h"],
    srcs = ["print_lib/src/printfFun.cpp"],
    deps = [
        ":add_lib",
    ],
)

cc_binary(
    name = "main",
    srcs = [
            "code/src/main.cpp",
            "code/inc/internal.h",
            ],
    deps = [
        ":print_lib",
    ],
    copts =[
        #"-Icode/inc/",
    ],
            
)
