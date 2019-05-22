#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表
#VPATH 目标文件的搜索路径，针对make不是gcc


cc = gcc -Iinclude
INCLUDE= -Iinclude
prom = output/exe
VPATH=src:include
obj = main.o getdata.o calc.o putdata.o

$(prom): $(obj)
	$(cc) $^ -o $@

main.o:define.h calc.h getdata.h putdata.h main.c

getdata.o:getdata.h getdata.c

calc.o:calc.h calc.c

putdata.o:putdata.h putdata.c
