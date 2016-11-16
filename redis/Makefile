UP_CC = g++
LIB = -lhiredis 
INC = -I /usr/local/include/hiredis
CPPS = example.cpp \

main:
	${UP_CC} -Wall -g -std=c++0x ${INC} ${CPPS} -o test ${LIB}

clean:
	@rm -rf *.o *.so
