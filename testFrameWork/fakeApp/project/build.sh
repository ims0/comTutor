cur_path=$(cd `dirname $0`; pwd)
echo ${cur_path}
cd ${cur_path}
echo `date "+%Y-%m-%d %H:%M:%S"`
## common func
rm_or_mkdir()
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
## var define
outFile=fakeApp
output_dir=${cur_path}/output
rm_or_mkdir $output_dir
if [[ $* =~ 'opt' ]];then
    build_type=Release
    build_dir=${cur_path}/build_release
else
    build_type=Debug
    build_dir=${cur_path}/build_debug
fi

if [[ $* =~ 'inc' ]];then
    echo "inc"
    if [ ! -d $build_dir ]; then
        mkdir -p $build_dir
        echo create:$build_dir
    fi
else
    rm_or_mkdir $build_dir
fi
## ccache
which ccache
if [ $? != 0 ];then
    echo "need install ccache"
    sudo apt install ccache 
fi

export CC="ccache gcc"
export CPP="ccache cpp"
export CXX="ccache g++"
export CCACHE_DIR="${cur_path}/ccache_tempfile"
## cmake
cd $build_dir
cmake -DCMAKE_BUILD_TYPE=$build_type -DCOVERAGE_FLAG=false -DPROJECT_PATH=$cur_path -DOUTPUT_PATH=$output_dir -DPROJECT_NAME=$outFile ..
## make
corenum=$(cat /proc/cpuinfo | grep processor | wc -l)
compile_start=`date +'%Y-%m-%d %H:%M:%S'`
make -j$corenum
compile_end=`date +'%Y-%m-%d %H:%M:%S'`
start_seconds=$(date --date="$compile_start" +%s);
end_seconds=$(date --date="$compile_end" +%s);
echo "compile_time:"$((end_seconds-start_seconds))" s"
##  run
cd $output_dir
./$outFile

run_end=`date +'%Y-%m-%d %H:%M:%S'`
run_seconds=$(date --date="$run_end" +%s);
echo "run_time:"$((run_seconds-end_seconds))" s"
