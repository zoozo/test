#include <zmq.hpp>
#include <iostream>
#include <boost/locale/encoding.hpp>
#define EXTRACT_U_1(p)  ((uint8_t)(*(p)))
#define ND_ISGRAPH(c)      ((c) > 0x20 && (c) <= 0x7E)

int main () {
    //  Prepare our context and socket
    zmq::context_t context;
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("ipc:///tmp/tradetron_spot");

    while (true) {


        //  Wait for next request from client
        unsigned char buffer [1024];
        unsigned char s;
        //char ch[2];
        char message[1024];
        unsigned char *cp = buffer;
        char *message_ptr = message;
        std::string msg;
        msg.reserve(1024);
        size_t length = socket.recv (buffer, 1024);
        std::cout << "Received ["<<length<<"]";
        int i = 0;
        while (length > 0) {
            s = EXTRACT_U_1(cp);
            cp++;
            length--;
			if (s == '\r') {
				/*
				 * Don't print CRs at the end of the line; they
				 * don't belong at the ends of lines on UN*X,
				 * and the standard I/O library will give us one
				 * on Windows so we don't need to print one
				 * ourselves.
				 *
				 * In the middle of a line, just print a '.'.
				 */
				if (length > 1 && EXTRACT_U_1(cp) != '\n')
					printf(".");
			} else {
                /*
                if(s == ''){
                    printf("^A");
       
				}else if (!ND_ISGRAPH(s) &&
						(s != '\t' && s != ' ' && s != '\n'))
					printf(".");
				else
                    */
                message[i++] = s;
			}

		}
        msg = reinterpret_cast<char*>(message);
        message_ptr += 72;
        printf("%s\n", message_ptr);
        std::cout<<strlen(message) << "["<<msg.size()<<"]"<<"["<<msg<<"]"<< "\n";



        //  Do some 'work'
        //sleep(1);

        //  Send reply back to client
        zmq::message_t reply (5);
        memcpy (reply.data (), "World", 5);
        socket.send (reply);
    }
    return 0;
}
