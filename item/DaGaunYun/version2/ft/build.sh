curr_path=$(cd `dirname $0`; pwd)
cd $curr_path

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

cd build
cmake ../cases
make
ln -sf ft/build/compile_commands.json ../../compile_commands.json 
../output/ft
lcov -q --directory . --capture -o cov_tem.info
lcov -q --extract cov_tem.info  *src* -o cov_test.info
genhtml --branch-coverage cov_test.info -o ../coverage_res
#/etc/lcovrc 
#lcov_branch_coverage = 1
