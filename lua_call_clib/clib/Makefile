$(warning A top-level warning)
CC = gcc
CFLAGS := -Wall -fPIC

LIB = mylib.so
OBJS = clib.o
LUA_LIB = -llua5.2 -ldl
LUA_INC_ROOT = /usr/include/lua5.2

INCLUDE_PATH = -I$(LUA_INC_ROOT)

all : $(LIB)
	rm -f $(OBJS)
	ls

$(LIB) : $(OBJS)
	$(CC) -shared -o $@ $(OBJS) $(LUA_LIB)

%.o : %.c
	@echo dep:$< obj:$@
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_PATH) 

clean:
	rm -f $(OBJS) $(LIB)


