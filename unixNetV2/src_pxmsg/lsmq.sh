#!/bin/bash
##########################################################
# File Name: lsmq.sh
# Author: ims
# Created Time: Mon 24 May 2021 11:13:15 PM CST
##########################################################
if [ ! -d /dev/mqueue ];then
    sudo mkdir /dev/mqueue
    sudo mount -t mqueue none /dev/mqueue
fi

ls /dev/mqueue
