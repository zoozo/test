#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

int main (int argc, char** argv)
{
    printf ("Connecting to hello world server…\n");
    std::string host = argv[1];
    std::string msg = argv[2];
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    
    //zmq_connect (requester, "tcp://128.110.5.67:5555");
    zmq_connect (requester, host.c_str());

    int request_nbr;
    for (request_nbr = 0; request_nbr < 1; request_nbr++) {
        sleep(1);
        char buffer [10];
        printf ("Sending Hello %d…\n", request_nbr);
        zmq_send (requester, msg.c_str(), msg.length(), 0);
        zmq_recv (requester, buffer, 10, 0);
        printf ("Received World %d:%s\n", request_nbr, buffer);
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
