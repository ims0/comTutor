#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表

CC = gcc
CFLAGS = -g -O0 -D_REENTRANT -Wall
LDFLAGS = -lrt -lpthread 
INCLUDEFLAGS =
IN_FILE=mqcom.h

PROGS = rwlock_demo mutex foo fakemutex mutex_type

.PHONY:all clean
all:${PROGS}


rwlock_demo:rwlock_demo.o 
	${CC} -o $@ $^ ${LDFLAGS}

mutex:mutex.o 
	${CC} -o $@ $^ ${LDFLAGS}

foo:foo.o mutex.o
	${CC} -o $@ $^ ${LDFLAGS}

fakemutex:fakemutex.o
	${CC} -o $@ $^ ${LDFLAGS}
fakemutex.o:mutex.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDEFLAGS) -DFAKE

mutex_type:mutex_type.o
	${CC} -o $@ $^ ${LDFLAGS}
#common
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
	rm -f ${PROGS} ${CLEANFILES} *.o a.out 
