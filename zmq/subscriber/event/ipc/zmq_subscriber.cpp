#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <event.h>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

void pair_callback(int fd, short what, void *arg)
{
    zmq::socket_t* socket = (zmq::socket_t*) arg;
    //zmq_getsockopt(socket, ZMQ_EVENTS, &events, &len);
    string pid = boost::lexical_cast<string>(getpid());

    int count = 0;
    if ( true )
    {
        while (1)
        {
            zmq::message_t message;
            socket->recv(&message);
            string text = std::string(static_cast<char*>(message.data()), message.size());

           
            cout << "[RECV] #"<<pid<<"#" << text << endl;
            zmq::message_t reply (5);
            if(text == "1"){
                memcpy (reply.data (), "11111", 5);
            }
            else{
                memcpy (reply.data (), "other", 5);
            }
            socket->send (reply);
            count++;
/*
            if(count % 10000 == 0) {
                unsigned long ms = std::chrono::system_clock::now().time_since_epoch() /
                    std::chrono::milliseconds(1);
                cout << "[RECV] #" << count << " at " << ms << ": \"" << text << "\"" << " " << message.size() << endl;
            }
*/
        }
    }
}


int main(int argc, char *argv[])
{
    string port = "6666";
    if(argc > 1){
        port = argv[1];
    }
    else{
    }
    struct event_base *evbase;
    struct event *ev;
    int pairfd;

    zmq::context_t context;
    zmq::socket_t socket(context, ZMQ_REP);
    //socket.connect("tcp://127.0.0.1:" + port);
    socket.bind("ipc:///tmp/test");
    cout << "connect to port:" <<port<<endl;

    pairfd = socket.getsockopt<int>(ZMQ_FD);

    evbase = event_base_new();
    ev = event_new(evbase, pairfd, EV_READ|EV_PERSIST, pair_callback, &socket);
    event_add(ev, NULL);

    event_base_dispatch(evbase);
    return 0;
}
