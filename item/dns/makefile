# when Invoking builtin recipe, will use those
CC = gcc
CXX = g++
CFLAGS = -g -O2 -Wall -D_XOPEN_SOURCE=500 -std=c99
CPPFLAGS = -g2 -Wall -std=c++11
LDFLAGS =

#自定义变量
INC_DIR = -I./

PROGS = dns

all:${PROGS}
	@echo all obj is new: ${PROGS}

${PROGS}:main.o  dns_lookup.o
	${CXX} -o $@ $^ ${LDFLAGS}
%.o:%.cpp
	$(CXX) -o $@ -c $< $(CPPFLAGS) $(INC_DIR)


.PHONY:all clean
clean:
	rm -f ${PROGS} *.o
