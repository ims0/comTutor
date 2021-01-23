#!/bin/bash
##########################################################
# File Name: check_str.sh
# Author: ims
# Created Time: 2020年12月13日 星期日 13时57分39秒
##########################################################
curr_path=$(cd `dirname $0`; pwd)
echo $curr_path

if [ $# != 1 ];then
    echo "need code dir arg!"
    exit 1
fi

if [ ! -d $1 ];then
    echo "dir not exist!"
    exit 2
fi


exe_name=check_exe
exe_file=$curr_path/$exe_name
src_dir=`pwd`/$1
logfile=$curr_path/log
cat /dev/null >$logfile

# compile file
set -x
gcc $curr_path/zh_str_check.c -std=c99 -O2 -o $exe_file
set +x

function callExeCheck(){
    $exe_file $1 $2 >>$logfile
    if [ $? != 0 ];then
        file $1 |tee -a $logfile
    fi
}


function doCheck() {
    fileName=$1
    if [ "${fileName##*.}"x = "cpp"x ]||[ "${fileName##*.}"x = "c"x ]||[ "${fileName##*.}"x = "cc"x ]||[ "${fileName##*.}"x = "h"x ];then
        callExeCheck $fileName cpp
    elif [ "${fileName##*.}"x = "lua"x ];then
        file $fileName |grep text > /dev/null
        if [ $? == 0 ];then
            callExeCheck $fileName lua
        fi
    elif [ "${fileName##*.}"x = "py"x ];then
        callExeCheck $fileName python
    fi
}


function check(){
    for element in `ls $1`
    do
        dir_or_file=$1"/"$element
        if [ -d $dir_or_file ] ;then
            check $dir_or_file
        else
            doCheck $dir_or_file
        fi
    done
}

$exe_file  $curr_path/test_file_dir/en.c cpp
if [[ $? == 0 ]];then
    echo "unit test c language en fail"
    exit 2
fi


getdir $src_dir
