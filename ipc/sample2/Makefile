UP_CC = g++
LIB = -lpthread -lrt
INC = 
CPPS = test.cpp \

main:
	${UP_CC} -Wall -g -std=c++0x ${INC} test1.cpp -o test1 ${LIB}
	${UP_CC} -Wall -g -std=c++0x ${INC} test2.cpp -o test2 ${LIB}

clean:
	@rm -rf *.o *.so
