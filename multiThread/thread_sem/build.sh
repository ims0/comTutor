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
cd coverage
gcc -fprofile-arcs -ftest-coverage ../sem_thread.c -o ../output/sem_thread -lpthread

../output/sem_thread
gcov ../sem_thread.c
lcov -d . -o 'sem_test.info' -b . -c

genhtml --branch-coverage  -o result sem_test.info
