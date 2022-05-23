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
    void *socket = zmq_socket (context, ZMQ_PAIR);
    int timeout = 1000;
    int hwm = 1;
    zmq_setsockopt (socket, ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
    zmq_setsockopt (socket, ZMQ_SNDTIMEO, &timeout, sizeof(timeout));
    zmq_setsockopt (socket, ZMQ_SNDHWM, &hwm, sizeof(hwm));
    
    //zmq_connect (socket, "tcp://128.110.5.67:5555");
    int rc = zmq_connect (socket, host.c_str());
    printf("RC:%d\n", rc);

    int request_nbr;
    for (request_nbr = 0; request_nbr < 1; request_nbr++) {
        char buffer [10];
        printf ("Sending Hello %d…\n", request_nbr);
        zmq_send (socket, msg.c_str(), msg.length(), 0);
        printf("%d\n", __LINE__);
        int ret = zmq_recv (socket, buffer, 10, 0);
        printf("%d\n", ret);
        printf ("Received World %d:%s\n", request_nbr, buffer);
        sleep(1);
    }
    zmq_ctx_destroy (&context);
    //    printf("%d\n", __LINE__);
    zmq_close (socket);
        printf("%d\n", __LINE__);
    return 0;
}
