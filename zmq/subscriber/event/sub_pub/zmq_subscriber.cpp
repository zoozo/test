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

            count++;
            if(count % 10000 == 0) {
                unsigned long ms = std::chrono::system_clock::now().time_since_epoch() /
                    std::chrono::milliseconds(1);
                cout << "[RECV] #" << count << " at " << ms << ": \"" << text << "\"" << " " << message.size() << endl;
            }
        }
    }
}

int main()
{
    struct event_base *evbase;
    struct event *ev;
    int pairfd;

    
    zmq::context_t context;

    zmq::socket_t socket(context, ZMQ_SUB);
    socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    socket.connect("tcp://127.0.0.1:6666");
    pairfd = socket.getsockopt<int>(ZMQ_FD);

    evbase = event_base_new();
    ev = event_new(evbase, pairfd, EV_READ|EV_PERSIST, pair_callback, &socket);
    event_add(ev, NULL);

    event_base_dispatch(evbase);
    return 0;
}
