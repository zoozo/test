#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/lexical_cast.hpp>

using namespace std;

static const string PUBLISH_ENDPOINT = "tcp://*:6666";

int main(int argc, char *argv[]) {

  // Create a publisher socket
  zmq::context_t context;
  //zmq::socket_type type = zmq::socket_type::publish;
  //zmq::socket_type type = zmq::socket_type::pub;
  zmq::socket_t socket (context, ZMQ_PUB);

  // Open the connection
  cout << "Binding to " << PUBLISH_ENDPOINT << "..." << endl;
  socket.bind(PUBLISH_ENDPOINT);

  // Pause to connect
  this_thread::sleep_for(chrono::milliseconds(1000));

  // Number of messages sent
  int count = 0;

  while(true) {

    // Current time in ms
    unsigned long long ms = chrono::system_clock::now().time_since_epoch() /
        chrono::milliseconds(1);

    count++;
    string text = "Hello at " + boost::lexical_cast<string>(count);

    // Create a message and feed data into it
    zmq::message_t message(text.length());
    memcpy(message.data(), text.c_str(), text.length());
    //message << text;

    // Send it off to any subscribers
    socket.send(message);

    if(count % 10000 == 0)
      cout << "[SENT] #" << count << " at " << ms << ": " << text << endl;

    //this_thread::sleep_for(chrono::microseconds(10));
  }

  // Unreachable, but for good measure
  socket.disconnect(PUBLISH_ENDPOINT);
  return 0;
}
