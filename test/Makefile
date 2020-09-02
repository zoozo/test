UP_CC = g++
STATIC_LIB = 
LIB = 
INC = 
#FLAGS =  -fno-rtti -g -W -Wextra -Wall -Wsign-compare -Wshadow -Wno-unused-parameter -Werror -std=c++11 -O2 -fno-omit-frame-pointer -momit-leaf-frame-pointer
FLAGS =    -g 
CPPS = test.cpp \

all: clean test

testcpp:
	${UP_CC} ${CPPS} -o$@  ${STATIC_LIB} ${INC} ${LIB} ${FLAGS}

clean:
	@rm -rf test *.o *.so
