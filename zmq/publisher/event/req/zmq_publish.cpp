#include <zmq.hpp>
#include <string>
#include <iostream>
using namespace std;
int main (int argc, char *argv[])
{
    //  Prepare our context and socket
    string port = argv[1];
    string send_msg = argv[2];
    int counter = atoi(argv[3]);
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    //socket.bind ("tcp://127.0.0.1:" + port);
    socket.connect ("tcp://127.0.0.1:" + port);

 
    zmq::message_t request (send_msg.size());
    memcpy (request.data (), send_msg.c_str(), send_msg.size());

    zmq::message_t reply;
    for(int i =0 ; i< counter ; i++){
        socket.send (request);

        //  Get the reply.
        socket.recv (&reply);
    }
    string text = std::string(static_cast<char*>(reply.data()), reply.size());
    std::cout << "Received message: " << text << std::endl;
    return 0;
}
