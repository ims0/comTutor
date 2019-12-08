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
cmake ../ -DCMAKE_BUILD_TYPE=Release


echo "start make"
make

echo "start running"
../output/ft

