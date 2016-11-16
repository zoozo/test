UP_CC = g++
LIB = -lzmq -L/usr/local/lib -levent
INC = -I /usr/local/include
CPPS = zmq_subscriber.cpp \

main:
	${UP_CC} ${CPPS} -o zmq_subscriber -Wall  -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP  -g ${INC} ${LIB} 

clean:
	@rm -rf *.o *.so
