#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <event.h>

using namespace std;

void pair_callback(int fd, short what, void *arg)
{
    zmq::socket_t* socket = (zmq::socket_t*) arg;
    //zmq_getsockopt(socket, ZMQ_EVENTS, &events, &len);

    int count = 0;
    if ( true )
    {
        while (1)
        {
            zmq::message_t message;
            socket->recv(&message);
            string text = std::string(static_cast<char*>(message.data()), message.size());

            cout << "[RECV] #" << text << endl;
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


int main()
{
    struct event_base *evbase;
    struct event *ev;
//    void *ctx, *pair;
//    size_t len;
    int pairfd;

   /* 
    ctx  = zmq_init(1);
    pair = zmq_socket(ctx, ZMQ_PULL);
    zmq_bind(pair, "tcp://127.0.0.1:6666");
    len = sizeof(pairfd);
    zmq_getsockopt(pair, ZMQ_FD, &pairfd, &len);
*/
    
    zmq::context_t context;
    zmq::socket_t socket(context, ZMQ_PULL);

    cout<<__LINE__<<endl;
    //socket.bind("tcp://127.0.0.1:6666");
    socket.connect("tcp://127.0.0.1:6666");
    cout<<__LINE__<<endl;
    pairfd = socket.getsockopt<int>(ZMQ_FD);

    evbase = event_base_new();
    //ev = event_new(evbase, pairfd, EV_READ|EV_PERSIST, pair_callback, pair);
    ev = event_new(evbase, pairfd, EV_READ|EV_PERSIST, pair_callback, &socket);
    event_add(ev, NULL);

    event_base_dispatch(evbase);
    return 0;
}
