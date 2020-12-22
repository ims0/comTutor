##########################################################
# File Name: check_str.sh
# Author: ims
# Created Time: 2020年12月13日 星期日 13时57分39秒
##########################################################
#!/bin/bash
curr_path=$(cd `dirname $0`; pwd)
echo $curr_path

if [ $# != 2 ];then
    echo "need path arg!"
    exit 1
fi

if [ ! -e $1 ];then
    echo "exe not exist!"
    exit 2
fi
if [ ! -d $2 ];then
    echo "dir not exist!"
    exit 3
fi
exe_file=$1
src_dir=$2

logfile=log
echo "" >$logfile

function doCheck() {
    file=$1
    if [ "${file##*.}"x = "cpp"x ]||[ "${file##*.}"x = "c"x ]||[ "${file##*.}"x = "cc"x ]||[ "${file##*.}"x = "h"x ];then
        file $dir_or_file |grep BOM > /dev/null
        if [ $? == 0 ];then
            sed  '1 s/^\xef\xbb\xbf//' $dir_or_file >nobom_tem
            dir_or_file=nobom_tem
        fi
        ./$exe_file $dir_or_file>>$logfile
        if [ $? == 1 ];then
            file $dir_or_file |tee -a $logfile
        fi
    elif [ "${file##*.}"x = "lua"x ];then
        file $dir_or_file |grep text > /dev/null
        if [ $? == 0 ];then
            ./$exe_file $dir_or_file>>$logfile  lua
            if [ $? == 1 ];then
                file $dir_or_file |tee -a $logfile
            fi
        fi
    fi
}

function getdir(){
    for element in `ls $1`
    do
        dir_or_file=$1"/"$element
        if [ -d $dir_or_file ] ;then
            getdir $dir_or_file
        else
            doCheck $dir_or_file
        fi
    done
}

getdir $src_dir
