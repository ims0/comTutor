#!/bin/bash
##########################################################
# File Name: cat.sh
# Author: ims
# Created Time: Sun 30 May 2021 07:37:47 PM CST
##########################################################
if [ $# -lt 1 ];then 
	echo "usage: $0 <posix_shm_name>"
	exit
fi

xxd /dev/shm/$1
