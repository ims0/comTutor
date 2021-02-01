#!/bin/bash

curr_path=$(cd `dirname $0`; pwd)
echo $curr_path

if [ $# -lt 1 ];then
    echo "need argument code_dir"
    echo "usage$ bash $0 code_dir [debug]"
    exit 1
fi

if [ ! -d $1 ];then
    echo "dir not exist!"
    exit 2
fi

if [[ $* =~ "debug" ]];then
    options="-DDEBUG -g3"
else
    options="-O2"
fi

exe_name=check_exe
exe_file=$curr_path/$exe_name
src_dir=`pwd`/$1 
src_file=zh_str_check.c
logfile=$curr_path/log
cat /dev/null >$logfile

# compile file
set -x
gcc $options -std=c99 $curr_path/$src_file -o $exe_file
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

#recursion
function check(){
    for element in `ls $1`
    do
        dir_or_file=$1"/"$element
        #echo $dir_or_file
        #[[ ! -e $dir_or_file ]]|| continue

        if [ -d $dir_or_file ] ;then
            check $dir_or_file
        else
            doCheck $dir_or_file
        fi
    done
}
#c language
$exe_file  $curr_path/test_file_dir/en.c cpp
if [[ $? != 0 ]];then
    echo "unit test c language en fail"
    exit 1
fi
$exe_file  $curr_path/test_file_dir/zh.c cpp > /dev/null
if [[ $? == 0 ]];then
    echo "unit test c language zh fail"
    exit 2
fi

#lua language
$exe_file  $curr_path/test_file_dir/en.lua lua
if [[ $? != 0 ]];then
    echo "unit test lua language en fail"
    exit 3
fi
$exe_file  $curr_path/test_file_dir/zh.lua lua > /dev/null
if [[ $? == 0 ]];then
    echo "unit test lua language zh fail"
    exit 4
fi

#python language
$exe_file  $curr_path/test_file_dir/en.py python
if [[ $? != 0 ]];then
    echo "unit test python language en fail"
    exit 5
fi
$exe_file  $curr_path/test_file_dir/zh.py python > /dev/null
if [[ $? == 0 ]];then
    echo "unit test python language zh fail"
    exit 6
fi

echo "======= unit test succ ======="
echo "check dir :"$src_dir
echo "======= The following file include chinese string ======="
check $src_dir
