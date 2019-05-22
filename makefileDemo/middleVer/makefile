#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表
#VPATH 目标文件的搜索路径，针对make不是gcc


cc = gcc
INCLUDE= -Iinclude
prom = output/exe
VPATH=src:include
obj = main.o getdata.o calc.o putdata.o

$(prom): $(obj)
	$(cc) $^ -o $@

main.o:define.h calc.h getdata.h putdata.h main.c
	$(cc) -c $(INCLUDE) $^

getdata.o:getdata.h getdata.c
	$(cc) -c $(INCLUDE) $^

calc.o:calc.h calc.c
	$(cc) -c $(INCLUDE) $^

putdata.o:putdata.h putdata.c
	$(cc) -c $(INCLUDE) $^
