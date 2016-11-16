#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <event.h>
#include <string>
#include <boost/lexical_cast.hpp>
#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace std;
using namespace rocksdb;


int main(int argc, char *argv[])
{
    zmq::context_t context;
    //zmq::socket_t socket(context, ZMQ_REP);
    zmq::socket_t socket(context, ZMQ_PAIR);
    //socket.connect("tcp://127.0.0.1:" + port);
    socket.bind("ipc:///tmp/test");
    DB* db;
    Options options;
    // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    // create the DB if it's not already present
    options.create_if_missing = true;

    std::string kDBPath = "/tmp/rocksdb_simple_example";


    // open DB
    Status s = DB::Open(options, kDBPath, &db);
    assert(s.ok());

    string val;
    while (true) {
        zmq::message_t request;

        //  Wait for next request from client
        socket.recv (&request);
        string key = std::string(static_cast<char*>(request.data()), request.size());
         s = db->Get(ReadOptions(), key, &val);
         cout<<val<<endl;


        //  Do some 'work'

        //  Send reply back to client
        //zmq::message_t reply (5);
        //memcpy (reply.data (), "World", 5);
        //socket.send (reply);
    }
    delete db;
    return 0;
}
