pwd
if test -d build
then
    echo ""
else
    mkdir build
fi
if test -d output
then
    rm -rf output/*
else
    mkdir output
fi
export CC=/usr/local/bin/gcc
export CXX=/usr/local/bin/g++
echo shell:$CC
echo shell:$CXX

cd build
cmake ../
make

cd ..
echo 'running'
./output/test_math
