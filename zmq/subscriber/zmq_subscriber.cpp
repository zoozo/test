#include <zmq.hpp>
#include <iostream>
#include <chrono>

using namespace std;

static const string PUBLISHER_ENDPOINT = "tcp://localhost:6666";

int main(int argc, char *argv[]) {

  // Create a subscriber socket
  zmq::context_t context;
  zmq::socket_t socket(context, ZMQ_SUB);

  socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

  cout << "Connecting to " << PUBLISHER_ENDPOINT << "..." << endl;
  socket.connect(PUBLISHER_ENDPOINT);

  // Number of messages received
  int count = 0;

  while(true) {

    // Receive (blocking call)
    zmq::message_t message;
    socket.recv(&message);

    count++;

    // Read as a string
    string text = std::string(static_cast<char*>(message.data()), message.size());

    if(count % 10000 == 0) {
      unsigned long ms = std::chrono::system_clock::now().time_since_epoch() /
          std::chrono::milliseconds(1);
      cout << "[RECV] #" << count << " at " << ms << ": \"" << text << "\"" << " " << message.size() << endl;
    }
  }

  // Unreachable, but for good measure
  socket.disconnect(PUBLISHER_ENDPOINT);
  return 0;
}
