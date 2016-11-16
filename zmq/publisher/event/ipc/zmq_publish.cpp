#include <zmq.hpp>
#include <string>
#include <iostream>
using namespace std;
int main (int argc, char *argv[])
{
    //  Prepare our context and socket
    string send_msg = argv[1];
    int counter = atoi(argv[2]);
    zmq::context_t context (1);
    //zmq::socket_t socket (context, ZMQ_REQ);
    zmq::socket_t socket (context, ZMQ_PAIR);

    //socket.bind ("tcp://127.0.0.1:" + port);
    socket.connect ("ipc:///tmp/test");

 
    zmq::message_t request (send_msg.size());
    memcpy (request.data (), send_msg.c_str(), send_msg.size());

    zmq::message_t reply;
    for(int i = 0 ; i < counter; i++){
        socket.send (request);
        //  Get the reply.
//        socket.recv (&reply);
    }
 //   string text = std::string(static_cast<char*>(reply.data()), reply.size());
 //   std::cout << "Received message: " << text << std::endl;
    return 0;
}
