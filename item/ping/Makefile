#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表
#make -r 取消内置的隐含规则:  builtin implicit recipe
#make -d 输出调试信息, 我们看到有大量的信息输出,其中就有查找是否需要更新Makefile 自身的
#make -r -d  调试信息一下就少多了,因为不包括那些隐含规则后, 就不用考虑是否生成Makefile 了.

#内置变量，默认规则会使用
# when Invoking builtin recipe, will use those
CC = gcc
CFLAGS = -g -Wall 
CPPFLAGS = 
LDFLAGS = -lpthread

#自定义变量
INC_DIR = -I./

PROGS = ping

all:${PROGS}
	@echo all obj is new: ${PROGS}

ping:ping.o 
	${CC} -o $@ $^ ${LDFLAGS}
ping.o:ping.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INC_DIR)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INC_DIR)


.PHONY:all clean
clean:
	rm -f ${PROGS} *.o
