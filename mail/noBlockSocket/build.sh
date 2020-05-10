##########################################################
# File Name: build.sh
# Author: ims
# Created Time: 2020年05月10日 星期日 15时11分25秒
##########################################################
#!/bin/bash

gcc server.c -o server -lpthread
gcc client.c -o client -lpthread
