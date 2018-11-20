UP_CC = g++
LIB = -lzmq
INC = 
CPPS = zmq_req.cpp \

main:
	${UP_CC} -Wall  -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP  -g ${INC} ${LIB} ${CPPS} -o zmq_req

clean:
	@rm -rf *.o *.so
