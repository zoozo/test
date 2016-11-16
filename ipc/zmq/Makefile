UP_CC = g++
STATIC_LIB = /usr/local/lib/librocksdb.a
LIB = -lpthread -lrt -lsnappy -lgflags -lz -lbz2 -lzmq
INC = -I /usr/local/include/rocksdb
FLAGS =  -O2 -std=c++11 -std=c++11  -DROCKSDB_PLATFORM_POSIX -DROCKSDB_LIB_IO_POSIX  -DOS_LINUX -fno-builtin-memcmp -DROCKSDB_FALLOCATE_PRESENT -DSNAPPY -DGFLAGS=google -DZLIB -DBZIP2 -DROCKSDB_MALLOC_USABLE_SIZE -DROCKSDB_PTHREAD_ADAPTIVE_MUTEX -DROCKSDB_BACKTRACE -march=native     -lpthread
CPPS = test.cpp \

all: clean test 

test:
	${UP_CC} $@.cpp -o$@  ${STATIC_LIB} ${INC} ${LIB} ${FLAGS}

clean:
	@rm -rf test *.o *.so
