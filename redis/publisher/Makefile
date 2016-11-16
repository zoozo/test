UP_CC = g++
LIB = -lhiredis -L/usr/local/lib -levent
INC = 
CPPS = redis_publisher.cpp \

main:
	${UP_CC} -Wall -g -std=c++0x ${INC} ${LIB} ${CPPS} -o redis_publisher

clean:
	@rm -rf *.o *.so
