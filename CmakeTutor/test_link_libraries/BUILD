cc_library(
    name = "add_lib",
    hdrs = ["add_lib/inc/add.h"],
    srcs = ["add_lib/src/addFun.cpp"],
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
    srcs = ["code/src/main.cpp"],
    deps = [
        ":print_lib",
    ],
)
