[toc]

## cmake 指定外部编译器的两种方法
+ export 设置环境变量
```
export CC=/usr/local/bin/gcc
export CXX=/usr/local/bin/g++
cmake /path/to/your/project
make
```
+ 把设置编译器的命令放在 project()之前
```    
SET(CMAKE_C_COMPILER "/home/public/local/bin/gcc")
SET(CMAKE_CXX_COMPILER "/home/public/local/bin/g++")
project(***)
```
### 设置编译选项的命令应放在project之后，否则无效

