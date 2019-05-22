cd ft
make -j4
if [ $?==0 ]; then
    make clean
else
    exit
fi


cd ../output
echo -e "\033[36;5m"
echo "============ Run ft ============="
echo -e "\033[0m"
./ft

