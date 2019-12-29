##########################################################
# File Name: build.sh
# Author: ims
# Created Time: 2019年12月29日 星期日 11时26分19秒
##########################################################
#!/bin/bash

g++    main.cpp info_log.cpp -o exe -Wall -Werror
./exe
