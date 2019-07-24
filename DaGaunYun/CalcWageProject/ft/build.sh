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

cd build
cmake ../cases
make
../output/ft
