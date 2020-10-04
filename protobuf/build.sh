#########################################################################
# File Name: build.sh
# Author: ims
# Created Time: 2019年01月11日 星期五 23时12分48秒
#########################################################################
:<<!
To get the source, download one of the release .tar.gz or .zip packages in the release page:
 https://github.com/protocolbuffers/protobuf/releases/latest
To build and install the C++ Protocol Buffer runtime and the Protocol Buffer compiler (protoc) execute the following:
 ./configure
 make
 make check
 sudo make install
 sudo ldconfig # refresh shared library cache.

By default, the package will be installed to /usr/local.
!

#!/bin/bash
set -x

pbdir=pbfile
if [ ! -d $pbdir ];then
    mkdir $pbdir
fi

protoc person.proto --cpp_out=$pbdir
protoc test.proto --cpp_out=$pbdir

echo "compiling~~~"

outfile="pbAddBook"
outdir="output"
exe=$outdir/$outfile

rm -rf $outdir 
mkdir $outdir 

g++ -g pbfile/*.c* *.c* -I$pbdir -o $exe `pkg-config --cflags --libs protobuf` 

echo "Running~~~"

./$exe
