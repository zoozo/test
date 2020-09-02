#include <iostream>
#include <fstream>
#include <vector>
#include "zasio/asio_client.hpp"
using namespace std;

class client : public zoozo::zasio::asio_client{
    public:
        void send(std::string& msg){
           get_socket().write_some(boost::asio::buffer(msg));
        }
};


void read_file(const string& file, vector<string>& data){
    ifstream myfile(file);
    string line;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            data.push_back(line);
        }
        myfile.close();
    }
}


int main(int argc ,char** argv){
    cout<<"start"<<endl;
    if(argc != 5){
        cout<<"ip port file usleep"<<endl;
        return 0;
    }

    string ip = argv[1];
    int port = atoi(argv[2]);
    string file = argv[3];
    int uslp = atoi(argv[4]);

    client c;
    c.connect(ip, port);


    cout<<"ip:"<<ip<<endl;
    cout<<"port:"<<port<<endl;
    cout<<"file:"<<file<<endl;
    cout<<"usleep:"<<uslp<<endl;

    vector<string> data;
    read_file(file, data);
    for(auto& line : data){
        c.send(line);
        usleep(uslp);
    }
    cout<<"*****done*****"<<endl;
}
