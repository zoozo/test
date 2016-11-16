#include <iostream>
#include <zmq.h>
#include <string.h>

using namespace std;

int main()
{
    void *ctx, *pair;
    zmq_msg_t msg;

    ctx  = zmq_init(1);
    pair = zmq_socket(ctx, ZMQ_PUB);
    //pair = zmq_socket(ctx, ZMQ_PUSH);
    zmq_connect(pair, "tcp://127.0.0.1:6666");

    zmq_msg_init_size(&msg, 6);
    cout<<"hello"<<endl;
    memcpy(zmq_msg_data(&msg), "hello", 6);
    zmq_sendmsg(pair, &msg, 0);
    zmq_msg_close(&msg);

    zmq_close(pair);
    zmq_term(ctx);
    return 0;
}
