CC = gcc
CXX = g++
CFLAGS = -Wall -pipe -D_DEBUG -DDEBUG -g -O0
LDFLAGS = -lstdc++
RM = /bin/rm -f
MODULE_INC = -I../curl-7.21.3/include -I../boost_1_45_0 -I./
MODULE_LIB = -L../boost_1_45_0/stage/lib
CFLAGS  += $(MODULE_INC)
LDFLAGS += $(MODULE_LIB)
LIBOBJS = Sender.o Recver.o
TARGET = sender recver
all: $(TARGET)
sender: Sender.o
	$(CXX) -o $@ $^ $(LDFLAGS)
recver: Recver.o
	$(CXX) -o $@ $^ $(LDFLAGS)
clean: 
	rm -f *.o
	rm -f $(TARGET)
# make rule
%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@	
%.o : %.cpp
	$(CC) $(CFLAGS) -c $^ -o $@	
install:
	cp -f $(TARGET) ../bin/

