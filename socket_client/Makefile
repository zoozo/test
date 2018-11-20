UP_CC = g++
STATIC_LIB = 
LIB = -lboost_system -lpthread
INC = 
FLAGS =  -fno-rtti -g -W -Wextra -Wall -Wsign-compare -Wshadow -Wno-unused-parameter -Werror -std=c++11 -O2 -fno-omit-frame-pointer -momit-leaf-frame-pointer
CPPS = test.cpp \

all: clean test 

test:
	${UP_CC} $@.cpp -o$@  ${STATIC_LIB} ${INC} ${LIB} ${FLAGS}

clean:
	@rm -rf test *.o *.so
