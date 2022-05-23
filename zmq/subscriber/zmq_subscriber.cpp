#include <zmq.hpp>
#include <iostream>
#include <chrono>

using namespace std;

//static const string PUBLISHER_ENDPOINT = "tcp://128.110.5.67:5556";
static const string PUBLISHER_ENDPOINT = "tcp://127.0.0.1:6666";

int main(int argc, char *argv[]) {

  string host = argv[1];
  // Create a subscriber socket
  zmq::context_t context;
  zmq::socket_t socket(context, ZMQ_SUB);
  //zmq::socket_t socket(context, ZMQ_PAIR);

  socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

  cout << "Connecting to " << host << "..." << endl;
  socket.bind(host.c_str());

  // Number of messages received
  int count = 0;

  while(true) {

    // Receive (blocking call)
    zmq::message_t message;
    socket.recv(&message);

    count++;

    // Read as a string
    string text = std::string(static_cast<char*>(message.data()), message.size());

    cout<<text<<endl;
      unsigned long ms = std::chrono::system_clock::now().time_since_epoch() /
          std::chrono::milliseconds(1);
      cout << "[RECV] #" << count << " at " << ms << ": \"" << text << "\"" << " " << message.size() << endl;
  }

  // Unreachable, but for good measure
  socket.disconnect(PUBLISHER_ENDPOINT);
  return 0;
}
