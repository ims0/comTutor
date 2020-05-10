if test -d build
then
    rm -rf build/*
else
    mkdir build
fi
if test -d output
then
    rm -rf output/*
else
    mkdir output
fi
outFile="OS"
cd build
cmake -DPROJECT_NAME=$outFile ..
make

# run
cd ../output
pwd

listenPort=3001
netstat -np |grep $listenPort
./$outFile $listenPort

