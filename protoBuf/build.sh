#########################################################################
# File Name: build.sh
# Author: ims
# Created Time: 2019年01月11日 星期五 23时12分48秒
#########################################################################
#!/bin/bash
protoc person.proto --cpp_out=pbfile
protoc test.proto --cpp_out=pbfile

echo "compiling~~~"

outfile="pbAddBook"
outdir="output"
outpath=$outdir/$outfile
if [ ! -d $outdir ];then
    mkdir $outdir 
elif [ ! -d "pbfile" ];then
    mkdir pbfile
fi
g++ -g pbfile/*.cc *.cpp -Ipbfile/ -I/usr/local/protobuf/include -L/usr/local/protobuf/lib  -lprotobuf -lpthread -o $outpath


echo "Running~~~"

./$outpath
