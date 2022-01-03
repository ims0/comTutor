#/usr/bash
#define 
RmOrMkdir()
{
    if [ $# -lt 1 ];then
        echo "need parh!"
        exit 1
    fi
    if [ -d $1 ];then
        rm -rf $1/*
    else
        mkdir -p $1
    fi
}

RmOrMkdir build
RmOrMkdir output

cd build
echo "start cmake"
pwd
#shell 中指定编译器，有效
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
#cmake -D是定义一个cmake使用的变量
cmake ../ -DCMAKE_BUILD_TYPE=Release -DOUTPUT_DIR=output


echo "start make"
make

echo "start running"
../output/ft

