#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include <boost/lexical_cast.hpp>
#include <pthread.h>

using namespace std;
#define ZMQ_PROTOC "inproc://test2"
//#define ZMQ_PROTOC "ipc:///tmp/test2"
#define PACKET_SIZE 1024

void job1(void *arg){
    zmq::context_t *context = (zmq::context_t *) arg;
    zmq::socket_t server(*context, ZMQ_REP);
    server.connect(ZMQ_PROTOC);
    char buffer[PACKET_SIZE];
    int length = 0;

    while(true){
        memset(buffer, 0 ,PACKET_SIZE);
        length = server.recv(buffer, PACKET_SIZE);
        cout<<"job1:"<<buffer<<endl;
        string msg = "ok";
        zmq::message_t reply (msg.length());
        memcpy(reply.data(), msg.c_str(), msg.length());
        server.send(reply);
    }

}
void job2(void *arg){
    zmq::context_t *context = (zmq::context_t *) arg;
    zmq::socket_t server(*context, ZMQ_REP);
    server.connect(ZMQ_PROTOC);
    char buffer[PACKET_SIZE];
    int length = 0;

    while(true){
        memset(buffer, 0 ,PACKET_SIZE);
        length = server.recv(buffer, PACKET_SIZE);
        cout<<"job2:"<<buffer<<endl;
        string msg = "ok";
        zmq::message_t reply (msg.length());
        memcpy(reply.data(), msg.c_str(), msg.length());
        server.send(reply);
    }

}
int main(int argc, char *argv[])
{
    zmq::context_t context(1);
    zmq::socket_t cli(context, ZMQ_REQ);
    //zmq::socket_t cli(context, ZMQ_PAIR);
    cli.bind(ZMQ_PROTOC);

    pthread_t zmq_t1;
    pthread_create (&zmq_t1, NULL, (void *(*)(void *))job1, (void*) &context);
    pthread_t zmq_t2;
    pthread_create (&zmq_t2, NULL, (void *(*)(void *))job2, (void*) &context);

    char buffer[PACKET_SIZE];
    int length = 0;

    while(true){
        sleep(1);
        string msg = "ruok";
        zmq::message_t request (msg.length());
        memcpy(request.data(), msg.c_str(), msg.length());
        cli.send(request);
        cout<<"sent"<<endl;
        length = cli.recv(buffer, PACKET_SIZE);

    }


    return 0;
}
