cur_path=$(cd `dirname $0`; pwd)
cd ${cur_path}
echo `date "+%Y-%m-%d %H:%M:%S"`
output_dir=output
if [[ $* =~ 'opt' ]];then
    build_type=Release
    build_dir=build_release
else
    build_type=Debug
    build_dir=build_debug
fi

#######################PREPARE WORKS##################################

function rm_or_mkdir()
{
    if [ $# -lt 1 ];then
        echo "function need arg";exit 1;
    fi
    if [ ! -d $1 ]; then
        mkdir -p $1
        echo create:$1
    else
        rm -rf $1/*
    fi
}
rm_or_mkdir $output_dir

if [[ $* =~ 'inc' ]];then
    echo "inc"
    if [ ! -d $build_dir ]; then
        mkdir -p $build_dir
        echo create:$build_dir
    fi
else
    rm_or_mkdir $build_dir
fi
####################### CCACHE ##################################
export CC="ccache gcc"
export CPP="ccache cpp"
export CXX="ccache g++"
export CCACHE_DIR="${cur_path}/ccache_tempfile"
######################################################
cd $build_dir
cmake -DCMAKE_BUILD_TYPE=$build_type  ..
## make
corenum=$(cat /proc/cpuinfo | grep processor | wc -l)
compile_start=`date +'%Y-%m-%d %H:%M:%S'`
make -j$corenum
compile_end=`date +'%Y-%m-%d %H:%M:%S'`
start_seconds=$(date --date="$compile_start" +%s);
end_seconds=$(date --date="$compile_end" +%s);
echo "compile_time:"$((end_seconds-start_seconds))" s"
####  run
cd ../output
./fakeApp

run_end=`date +'%Y-%m-%d %H:%M:%S'`
run_seconds=$(date --date="$run_end" +%s);
echo "run_time:"$((run_seconds-end_seconds))" s"
