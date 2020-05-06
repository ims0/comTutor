##########################################################
# File Name: regexpReplace.sh
# Author: ims
# Created Time: 2019年11月29日 星期五 23时23分05秒
##########################################################
#!/bin/bash
if [ $# -lt 1 ];then
    echo "need path arg"
    exit
fi

#var define
path=$1
binaryOperator="binaryOperator"

#func define
showGreen()
{
  echo -e "\e[1;32m$1\e[0m"
}
showRed()
{
  echo -e "\e[1;31m$1\e[0m"
}
Grep()
{
    if [ $# -lt 1 ];then
        return 
    else
        GrepRes=(`grep -P $1 -rl $path --include=*.{cpp,c,h}`)
    fi
}
Replace()
{
    if [ $# -lt 1 ];then return;fi
    from=$1; to=$2
    echo "--Scanning file which contain $from"
    Grep $from
    if [ ${#GrepRes[*]} -eq 0 ];then
        showRed "--no match any file"
        return
    fi
    echo --match fileList:
    echo ${GrepRes[*]}

    if [ $from == '\t' ];then
        echo "--1start processing: [$from] be replace [    ]"
        sed -i -E "s/$from/    /g" ${GrepRes[*]}
    elif [ $to == $binaryOperator ];then
        echo "--2start binaryOperator processing: [$from] be replace [ $from ]"
        sed -i -E "s/(\w+)\s*$from\s*(\w+)/\1 $from \2/g" ${GrepRes[*]}
        return
    else
        echo "--3start processing: [$from] be replace [$to]"
        sed -i -E "s/$from/$to/g" ${GrepRes[*]}
    fi
    # check
    Grep $from
    if [ ${#GrepRes[*]} -eq 0 ];then
        showGreen "--Check result ok"
    else
        showRed "--those file:${GrepRes[@]} process failed!"
    fi
    echo "--end"
}

#cmd
echo "--path:$path"

Replace '\t' 
Replace '=' $binaryOperator
Replace '>' $binaryOperator
Replace '<' $binaryOperator
Replace '<<' $binaryOperator
Replace '>>' $binaryOperator
Replace '==' $binaryOperator


#
Replace '\s+$'
