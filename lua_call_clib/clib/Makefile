CC = gcc
CFLAGS := -Wall -fPIC

LIB = mylib.so
OBJS = clib.o

luaver := `lua -v 2>&1| awk '{print $$2}'`
luaver_num := $(shell a=$(luaver);echo $${a%.*})
LUA_VERSION:=lua$(luaver_num)

$(info current lua version:$(LUA_VERSION))
ifeq ($(LUA_VERSION),lua5.1)
	COMPILE_MACRO= -DLUA_LOW_VER
endif
LUA_LIB = -l$(LUA_VERSION) -ldl
LUA_INC_ROOT = /usr/include/$(LUA_VERSION)

INCLUDE_PATH = -I$(LUA_INC_ROOT)

all : $(LIB)
	rm -f $(OBJS)
	ls
	@echo "'baer make' produce compile_commands.json"

$(LIB) : $(OBJS)
	$(CC) -shared -o $@ $(OBJS) $(LUA_LIB)

%.o : %.c
	@echo dep:$< obj:$@
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_PATH) $(COMPILE_MACRO)

clean:
	rm -f $(OBJS) $(LIB)

.PHONY: clean
