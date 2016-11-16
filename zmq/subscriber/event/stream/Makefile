UP_CC = g++
LIB = -lzmq -L/usr/local/lib -levent
INC = 
CPPS = zmq_subscriber.cpp \

main:
	${UP_CC} ${CPPS} -Wall  -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP  -g ${INC} ${LIB} -o zmq_subscriber

clean:
	@rm -rf *.o *.so
