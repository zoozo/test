#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include <boost/lexical_cast.hpp>
#include <pthread.h>
static std::string
s_recv (zmq::socket_t & socket) {

    zmq::message_t message;
    socket.recv(&message);

    return std::string(static_cast<char*>(message.data()), message.size());
}
static bool
s_send (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message);
    return (rc);
}

//  Sends string as 0MQ string, as multipart non-terminal
static bool
s_sendmore (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message, ZMQ_SNDMORE);
    return (rc);
}


using namespace std;
#define ZMQ_PROTOC_FRONT "inproc://frontend"
#define ZMQ_PROTOC_BACK "inproc://backend"
//#define ZMQ_PROTOC "ipc:///tmp/test2"
#define PACKET_SIZE 1024

void client1(void *arg){
    zmq::context_t *context = (zmq::context_t *) arg;
    zmq::socket_t cli(*context, ZMQ_REQ);
    zmq_setsockopt (cli, ZMQ_IDENTITY, "CLI1", 4);
    cli.connect(ZMQ_PROTOC_FRONT);
    char buffer[PACKET_SIZE];
    int length = 0;

    while(true){
        sleep(1);
        string msg = "cli 1 msg";
        zmq::message_t request (msg.length());
        memcpy(request.data(), msg.c_str(), msg.length());
        cli.send(request);
        cout<<"cli1 sent"<<endl;
        length = cli.recv(buffer, PACKET_SIZE);
    }

}
void client2(void *arg){
    zmq::context_t *context = (zmq::context_t *) arg;
    zmq::socket_t cli(*context, ZMQ_REQ);
    zmq_setsockopt (cli, ZMQ_IDENTITY, "CLI2", 4);
    cli.connect(ZMQ_PROTOC_FRONT);
    char buffer[PACKET_SIZE];
    int length = 0;

    while(true){
        sleep(1);
        string msg = "cli 2 msg";
        zmq::message_t request (msg.length());
        memcpy(request.data(), msg.c_str(), msg.length());
        cli.send(request);
        cout<<"cli2 sent"<<endl;
        length = cli.recv(buffer, PACKET_SIZE);
    }

}
void worker1(void *arg){
    zmq::context_t *context = (zmq::context_t *) arg;
    zmq::socket_t cli(*context, ZMQ_REQ);
    zmq_setsockopt (cli, ZMQ_IDENTITY, "WRK1", 4);
    cli.connect(ZMQ_PROTOC_BACK);
    char buffer[PACKET_SIZE];
    int length = 0;
    s_send(cli, "READY");

    while(true){
        std::string address = s_recv(cli);
        {
            std::string empty = s_recv(cli);
            assert(empty.size() == 0);
        }
        std::string request = s_recv(cli);
        std::cout << "Worker 1: " << request << std::endl;

        s_sendmore(cli, address);
        s_sendmore(cli, "");
        s_send(cli, "OK");
    }
}
void worker2(void *arg){
    zmq::context_t *context = (zmq::context_t *) arg;
    zmq::socket_t cli(*context, ZMQ_REQ);
    zmq_setsockopt (cli, ZMQ_IDENTITY, "WRK2", 4);
    cli.connect(ZMQ_PROTOC_BACK);
    char buffer[PACKET_SIZE];
    int length = 0;
    s_send(cli, "READY");

    while(true){
        std::string address = s_recv(cli);
        {
            std::string empty = s_recv(cli);
            assert(empty.size() == 0);
        }
        std::string request = s_recv(cli);
        std::cout << "Worker 2: " << request << std::endl;

        s_sendmore(cli, address);
        s_sendmore(cli, "");
        s_send(cli, "OK");
    }

}
int main(int argc, char *argv[])
{
    zmq::context_t context_front(1);
    zmq::context_t context_back(1);
    zmq::socket_t frontend(context_front, ZMQ_ROUTER);
    zmq::socket_t backend(context_back, ZMQ_ROUTER);
    frontend.bind(ZMQ_PROTOC_FRONT);
    backend.bind(ZMQ_PROTOC_BACK);

    pthread_t zmq_t1;
    pthread_create (&zmq_t1, NULL, (void *(*)(void *))client1, (void*) &context_front);
    pthread_t zmq_t2;
    pthread_create (&zmq_t2, NULL, (void *(*)(void *))client2, (void*) &context_front);
    pthread_t zmq_t3;
    pthread_create (&zmq_t3, NULL, (void *(*)(void *))worker1, (void*) &context_back);
    pthread_t zmq_t4;
    pthread_create (&zmq_t4, NULL, (void *(*)(void *))worker2, (void*) &context_back);

    char buffer[PACKET_SIZE];
    int length = 0;

    int work_size = 0;
    while(work_size < 2){
        cout<<"work init"<<endl;
        std::string client_addr = s_recv(backend);
        cout<<"work:"<<client_addr<<endl;
        work_size++;
    }
    cout<<"work size:"<<work_size<<endl;
    return 0;
    while(work_size < 2){
        cout<<"work init"<<endl;
        std::string client_addr = s_recv(backend);
        if (client_addr.compare("READY") != 0) {

            {
                std::string empty = s_recv(backend);
                assert(empty.size() == 0);
            }

            std::string reply = s_recv(backend);
            s_sendmore(frontend, client_addr);
            s_sendmore(frontend, "");
            s_send(frontend, reply);
            work_size++;
        }
    }
    while(true){
        cout<<"get task"<<endl;
        std::string client_addr = s_recv(frontend);

        std::string empty = s_recv(frontend);
        assert(empty.size() == 0);

        std::string reply = s_recv(frontend);


        s_sendmore(backend, "WRK2");
        s_sendmore(backend, "");
        s_sendmore(backend, client_addr);
        s_sendmore(backend, "");
        s_send(backend, reply);
    }


    return 0;
}
