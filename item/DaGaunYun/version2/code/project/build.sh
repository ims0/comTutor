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
cmake ..
make
../output/calcWage "袭人" 1

../output/calcWage "袭人" 1 "贾母"
../output/calcWage "袭人" 1 {"贾母","宝玉","贾政"}
../output/calcWage "袭人" 1 {"贾母","宝玉","贾政"} {5,8,9} 
