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
<<<<<<< Updated upstream
whiteSpaceReg='\s+$'
tabFlag='tab'

#func define
 grep -P '\s+$' -rl testdir/
Replace()
=======
whiteSpaceReg='\s\+$'
tabFlag='tab'

#func define
function Replace()
>>>>>>> Stashed changes
{
    from=$1; to=$2
    echo "--Scanning file which contain $from"
    grep -P $from -rl $path --include=*.{cpp,c,h}

    echo "--start processing: [$from] be replace [$to]"
    if [ $# -lt 2 ];then
        sed -i "s/$from//g" `grep -P $from -rl $path --include=*.{cpp,c,h}`
    elif [ $to == $tabFlag ];then
        sed -i "s/$from/    /g" `grep -P $from -rl $path --include=*.{cpp,c,h}`
    else
        sed -i "s/$from/$to/g" `grep -P $from -rl $path --include=*.{cpp,c,h}`
    fi
    echo "--Check result"
    grep -P $from -rl $path --include=*.{cpp,c,h}
    echo "--end"
}

#cmd
echo "--path $path"
Replace $whiteSpaceReg
