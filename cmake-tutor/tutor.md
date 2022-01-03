[toc]<!-- TOC -->autoauto- [cmake 指定外部编译器的两种方法](#cmake-指定外部编译器的两种方法)auto- [CMakeLists 中把设置编译器的命令放在 project()之前](#cmakelists-中把设置编译器的命令放在-project之前)auto- [设置编译选项的命令应放在project之后，](#设置编译选项的命令应放在project之后)auto    - [project 功能：检测编译器，清空各个环境变量，设置app name](#project-功能检测编译器清空各个环境变量设置app-name)auto- [查看include的默认搜索路径](#查看include的默认搜索路径)auto- [不使用系统默认头文件](#不使用系统默认头文件)auto- [显示 .c 所有的头文件依赖: -M](#显示-c-所有的头文件依赖--m)autoauto<!-- /TOC -->

[toc]
## cmake 指定外部编译器的两种方法
+ shell 脚本中 export 设置环境变量
```
export CC=/usr/local/bin/gcc
export CXX=/usr/local/bin/g++
cmake /path/to/your/project
make
```
## CMakeLists 中把设置编译器的命令放在 project()之前

tip: 路径字符串两端不能有空格
```    
SET(CMAKE_C_COMPILER "/home/public/local/bin/gcc")
SET(CMAKE_CXX_COMPILER "/home/public/local/bin/g++")
project(***)
```

## 设置编译选项的命令应放在project之后，

```
set(CMAKE_C_FLAGS            " -Wall -Wfatal-errors -std=c99    " )
set(CMAKE_C_FLAGS_DEBUG      " -g3                              " )
set(CMAKE_C_FLAGS_RELEASE    " -O3                              " )
set(CMAKE_CXX_FLAGS          " -Wall -Wfatal-errors -std=c++11  " )
set(CMAKE_CXX_FLAGS_DEBUG    " -g3                              " )
set(CMAKE_CXX_FLAGS_RELEASE  " -O3                              " )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

### project 功能：检测编译器，清空各个环境变量，设置app name
project(app) 

## 查看include的默认搜索路径

`gcc -xc -E -v -`
## 不使用系统默认头文件
    c 使用选项 -nostdinc 
    C++  的是 -nostdin C++

## 显示 .c 所有的头文件依赖: -M
