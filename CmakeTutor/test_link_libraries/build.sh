#!/bin/bash

# func

RmMkdir()
{
    if [ $# -lt 1 ];then
        echo "RmMkdir need arg";exit
    fi
    if [ -d $1 ];then
        rm -rf $1/*
    else
        mkdir -p $1
    fi
}

RmMkdir build

export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
echo "env var CC:$CC"
echo "env var CXX:$CXX"

cd build
cmake ../ -DPROJECT_NAME=test_depend_lib
make

echo 'running'
./test_depend_lib
