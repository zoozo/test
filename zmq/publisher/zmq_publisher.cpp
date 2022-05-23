#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/lexical_cast.hpp>

using namespace std;

/*
./zmq_publisher "tcp://127.0.0.1:4557" ssss
*/

int main(int argc, char *argv[]) {

  // Create a publisher socket
  zmq::context_t context;
  //zmq::socket_type type = zmq::socket_type::publish;
  //zmq::socket_type type = zmq::socket_type::pub;
  zmq::socket_t socket (context, ZMQ_PUB);

  // Open the connection
  cout << "Binding to " << argv[1] << "..." << endl;
  //socket.connect(argv[1]);
  socket.connect(argv[1]);

  // Pause to connect
  this_thread::sleep_for(chrono::milliseconds(1000));

  // Number of messages sent

    string text = argv[2];

    // Create a message and feed data into it
    zmq::message_t message(text.length());
    memcpy(message.data(), text.c_str(), text.length());
    //message << text;

    // Send it off to any subscribers
    socket.send(message);


  // Unreachable, but for good measure
  socket.disconnect(argv[1]);
  return 0;
}
