UP_CC = g++
LIB = -lzmq
INC = 
CPPS = zmq_test.cpp \

main:
	${UP_CC} -Wall  -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP  -g ${INC} ${LIB} ${CPPS} -o zmq_test

clean:
	@rm -rf *.o *.so
