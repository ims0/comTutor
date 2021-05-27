#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表

CC = gcc
CFLAGS = -g -O2 -Wall
LDFLAGS =  
INCLUDEFLAGS =
IN_FILE=

PROGS = map_normalfile1 map_normalfile2

all:${PROGS}

map_normalfile1:map_normalfile1.o 
	${CC} -o $@ $^ ${LDFLAGS}

map_normalfile2:map_normalfile2.o 
	${CC} -o $@ $^ ${LDFLAGS}



%.o:%.c $(IN_FILE)
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDEFLAGS)


#https://segmentfault.com/a/1190000000349917

.PHONY:all clean
clean:
	rm -f ${PROGS} ${CLEANFILES} *.o
