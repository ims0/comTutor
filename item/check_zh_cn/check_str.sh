##########################################################
# File Name: check_str.sh
# Author: ims
# Created Time: 2020年12月13日 星期日 13时57分39秒
##########################################################
#!/bin/bash
curr_path=$(cd `dirname $0`; pwd)
echo $curr_path

if [ $# != 1 ];then
    echo "need path arg!"
    exit 1
fi

if [ ! -d $1 ];then
    echo "dir not exist!"
    exit 2
fi
logfile=log
echo "" >$logfile

function doCheck() {
    file=$1
    if [ "${file##*.}"x = "cpp"x ]||[ "${file##*.}"x = "c"x ];then
        echo $dir_or_file
        ./a.out $dir_or_file>>$logfile
        echo $?
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

getdir $1
