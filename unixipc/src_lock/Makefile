#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表

CC = gcc
CFLAGS = -g -O2 -D_REENTRANT -Wall
LDFLAGS = -lrt -lpthread 
INCLUDEFLAGS =
IN_FILE=mqcom.h

PROGS = rwlock_demo

.PHONY:all clean
all:${PROGS}


rwlock_demo:rwlock_demo.o 
	${CC} -o $@ $^ ${LDFLAGS}

%.o:%.c $(IN_FILE)
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDEFLAGS)

%.d : %.c
	@set -e; \
	gcc -MM $@ > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include seq.d
#https://segmentfault.com/a/1190000000349917

clean:
	rm -f ${PROGS} ${CLEANFILES} *.o
