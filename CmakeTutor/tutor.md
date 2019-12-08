[toc]

## cmake 指定外部编译器的两种方法
+ shell 脚本中 export 设置环境变量
```
export CC=/usr/local/bin/gcc
export CXX=/usr/local/bin/g++
cmake /path/to/your/project
make
```
+ CMakeLists 中把设置编译器的命令放在 project()之前

    路径字符串两端不能有空格
```    
SET(CMAKE_C_COMPILER "/home/public/local/bin/gcc")
SET(CMAKE_CXX_COMPILER "/home/public/local/bin/g++")
project(***)
```

## 设置编译选项的命令应放在project之后，否则无效

```
set(CMAKE_C_FLAGS            " -Wall -Wfatal-errors -std=c99    " )
set(CMAKE_C_FLAGS_DEBUG      " -g3                              " )
set(CMAKE_C_FLAGS_RELEASE    " -O3                              " )
set(CMAKE_CXX_FLAGS          " -Wall -Wfatal-errors -std=c++11  " )
set(CMAKE_CXX_FLAGS_DEBUG    " -g3                              " )
set(CMAKE_CXX_FLAGS_RELEASE  " -O3                              " )
```

project(app)
