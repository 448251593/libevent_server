
CC = g++

LIBS = -L../libevent_lib_install/lib -levent -levent_core -levent_extra -levent_pthreads -lpthread 
LIBS +=  -lrt 
LIBS += -L/usr/lib64/mysql/


CFLAGS = -I../libevent_lib_install/include/event2/ -I../libevent_lib_install/include/ -I./inc  -std=c++0x -pthread 
CFLAGS += -I/opt/tcpserver/ -I/opt/lampp/include 
CFLAGS +=  -I/home/bcg/

EXECUT = exelibevent

C_SRCS += $(wildcard ./src/*.cpp)
ALL_OBJ += $(patsubst ./src/%.cpp, ./src/%.o, $(C_SRCS))
	
all: $(EXECUT)
	
$(EXECUT): $(ALL_OBJ)
	$(CC) $(ALL_OBJ) $(LIBS) $(CFLAGS)   -o $(EXECUT)

./src/%.o:./src/%.cpp
	$(CC) $(CFLAGS)  -c $< -o $@

clean:  
	-rm -f $(ALL_OBJ)
	-rm -f $(EXECUT)
