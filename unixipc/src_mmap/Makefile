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
CFLAGS = -g -O2 -Wall -D_XOPEN_SOURCE=500 -std=c99
CPPFLAGS = 
LDFLAGS =  -lrt

#自定义变量
INC_DIR = -I./

PROGS = mmap_file_wt mmap_file_rd mmap_posix_wt  mmap_posix_rd

all:${PROGS}
	@echo all obj is new: ${PROGS}

# 这里只有 mmap_file_wt, 使用了指定的规则，其它用 builtin
mmap_file_wt:mmap_file_wt.o 
	${CC} -o $@ $^ ${LDFLAGS}
%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INC_DIR)


.PHONY:all clean
clean:
	rm -f ${PROGS} *.o
