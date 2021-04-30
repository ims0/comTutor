$(warning A top-level warning)
CC = gcc
CFLAGS := -Wall -fPIC

LUA_VERSION=lua5.1
COMPILE_MACRO=-DLUA_LOW_VER
LIB = mylib.so
OBJS = clib.o
LUA_LIB = -l$(LUA_VERSION) -ldl
LUA_INC_ROOT = /usr/include/$(LUA_VERSION)

INCLUDE_PATH = -I$(LUA_INC_ROOT)

all : $(LIB)
	rm -f $(OBJS)
	ls

$(LIB) : $(OBJS)
	$(CC) -shared -o $@ $(OBJS) $(LUA_LIB)

%.o : %.c
	@echo dep:$< obj:$@
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_PATH) $(COMPILE_MACRO)

clean:
	rm -f $(OBJS) $(LIB)


