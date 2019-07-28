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
lcov --directory . --capture -o cov_tem.info
lcov --extract cov_tem.info  *src* -o cov_test.info
genhtml --branch-coverage cov_test.info -o ../coverage_res
