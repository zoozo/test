UP_CC = g++
LIB = -lhiredis -L/usr/local/lib -levent
INC = 
CPPS = redis_subscriber.cpp \

main:
	${UP_CC} -Wall -g -std=c++0x ${INC} ${LIB} ${CPPS} -o redis_subscriber

clean:
	@rm -rf *.o *.so
