UP_CC = g++
LIB = -lzmq
INC = 
CPPS = zmq_publish.cpp \

main:
	${UP_CC} ${CPPS} -o zmq_publisher -Wall  -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP  -g ${INC} ${LIB} 

clean:
	@rm -rf *.o *.so
