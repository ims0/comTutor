#########################################################################
# File Name: build.sh
# Author: ims
# Created Time: 2019年07月27日 星期六 22时22分29秒
#########################################################################
#!/bin/bash
if test -d build
then
    rm -rf build/*
else
    mkdir build
fi


if test -d output
then
    rm -rf output/*
else
    mkdir output
fi
if test -d coverage
then
    rm -rf coverage/*
else
    mkdir coverage
fi

cp sem_thread.c build
cd build

gcc ../sem_thread.c  -g -fprofile-arcs -ftest-coverage -o ../output/sem_thread -lpthread

../output/sem_thread
#create sem_thread.c.gcov  dont use it when use lcov
#gcov -b sem_thread.c
#create html result
#-d: .gcno .gcda 所在的文件夹，注意这里有个“.”，是从当前文件夹中获取数据
#(that's where the counter files ending with .da will be stored)
#-c: lcov 的一个操作，表示要去捕获覆盖率数据
#-o: 输出文件
lcov -d .  -c -o 'sem_test.info'
genhtml --branch-coverage sem_test.info -o ../coverage
