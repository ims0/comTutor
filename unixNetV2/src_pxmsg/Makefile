#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表

CC = gcc
CFLAGS = -g -O2 -D_REENTRANT -Wall
LIBS = -lrt -lpthread 
IN_FILE=mqcom.h

PROGS = mqsend mqrcv

all:${PROGS}

mqsend:mqsend.o $(IN_FILE)
	${CC} ${CFLAGS} $^ -o $@ ${LIBS}

mqrcv:mqrcv.o $(IN_FILE)
	${CC} ${CFLAGS} $^ -o $@ ${LIBS}

clean:
	rm -f ${PROGS} ${CLEANFILES} *.o
