UP_CC = g++
LIB = -lzmq -L/usr/local/lib 
INC = 
CPPS = zmq_rep.cpp \

main:
	${UP_CC} -Wall  -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP  -g ${INC} ${LIB} ${CPPS} -o zmq_test

clean:
	@rm -rf *.o *.so
