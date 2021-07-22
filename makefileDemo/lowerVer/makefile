#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表
#VPATH 目标文件的搜索路径，针对make不是gcc


cc = gcc
PROGS = exe
VPATH=src:include
obj = main.o getdata.o calc.o putdata.o
libfile = getdata.o calc.o putdata.o

PROGS = share_exe static_exe libshare.so libstatic.a
all: ${PROGS} 

share_exe:main.o libshare.so
	$(CC) -o $@ $< -lshare -L./
	#export LD_LIBRARY_PATH=./; ./$@

static_exe:main.o libstatic.a
	$(CC) -o $@ $^


libshare.so: $(libfile)
	$(cc) -o $@  -shared $^

libstatic.a: $(libfile)
	ar cvr $@ $^

main.o:main.c define.h calc.h getdata.h putdata.h
	$(cc) -o main.i -E $<
	$(cc) -o main.s -S $<
	$(cc) -o main.o -c $<

getdata.o:getdata.h 
putdata.o:putdata.h
calc.o:calc.h 


.PHONY:all clean
cl clean:
	rm -f ${PROGS} *.o  *.so core *.i *.s
