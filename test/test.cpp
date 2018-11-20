#include <iostream>
using namespace std;

int UDPrecv(int sock, char *buf, size_t len)
{
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
    /* check parameter */
//    if ((sock < 0) || (! buf) || (! len) || (*len <= 0))
 //       return -1;

    int recvLen = recvfrom(sock, buf, len, 0, (struct sockaddr *)&peer_addr, &peer_addr_len);
    //if (ip)    strcpy(ip, inet_ntoa(sa.sin_addr));
    //if (port)  *port = ntohs(sa.sin_port);
    return recvLen;
}

int UDPConnect(const char *bindip, const char *groupip, int port )//{{{
{
    int s;
    struct sockaddr_in server;

    if ((s = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0)
    {
        perror("socket()");
        return -1;
    }

    /* Set up the server name */
    server.sin_family      = PF_INET;            /* Internet Domain    */
    server.sin_port        = htons(port);               /* Server Port        */
    server.sin_addr.s_addr =  htonl(INADDR_ANY);
    //server.sin_addr.s_addr =  inet_addr( "128.110.3.230" );
    //server.sin_addr.s_addr =  inet_addr( "10.4.101.67" );

    //int reuseaddr = 1;
    //socklen_t  reuseaddr_len = sizeof(reuseaddr);
    //setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&reuseaddr,reuseaddr_len);
    int err = bind(s,(struct sockaddr*)&server, sizeof(server)) ;
    if(err < 0){
        perror("bind error!!!\n");
        return -1;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(groupip);
    if( bindip == NULL || strlen(bindip) == 0 )
        mreq.imr_interface.s_addr = INADDR_ANY; // use DEFAUL
    else
        mreq.imr_interface.s_addr = inet_addr( bindip );

    err = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
    if (err < 0)
    {
        perror("setsockopt():IP_ADD_MEMBERSHIP");
        return -4;
    }
    else{
        //plog(std::string("join UDP group:") + groupip);
    }
    return s;
}//}}}



int main(int argc, char** argv) {
    int _socket;
    fd_set  rset;
    struct timeval  timeout;

    size_t msg_len = 0;
    char buf[1024];
    _socket = UDPConnect(bindip.c_str(), groupip.c_str(), port);
    while(true){
        //boost::shared_ptr<char> buf(new char[1024]);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        FD_ZERO(&rset);
        FD_SET(_socket, &rset);
        int rc = select(_socket+1, &rset, NULL, NULL, &timeout);
        if(rc > 0){
            if(!FD_ISSET(_socket, &rset)){
                continue;
            }
            //msg_len = UDPrecv(_socket, buf, buf_len);
            if(msg_len <= 0){
                continue;
            }
            buf[msg_len] = 0;
            cout<<"recv:"<<buf<<endl;
        }else{
            cout<<"rc:"<<rc<<endl;
        }
    }
}
