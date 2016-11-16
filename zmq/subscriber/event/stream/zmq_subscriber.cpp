#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <event.h>
#include <string>
#include <list>
#include <map>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <thread>

using namespace std;
map<string, vector<string> > g_fix_map;
void GetCheckSum(const char *buf, long bufLen, char *obuf )
{
    long idx;
    unsigned int cks;

    for( idx = 0L, cks = 0; idx < bufLen; cks += (unsigned int)buf[ idx++ ] );
    sprintf( obuf, "%03d", (unsigned int)( cks % 256 ) );
}


bool message_parse(string& recv_msg, list<string>& send_list){

    map<string,string> fix_map;
    boost::char_separator<char> sep("");
    string checksum, str;
    char buf[11];
    list<string> msg_list;
    boost::tokenizer<boost::char_separator<char> > tokens(recv_msg, sep);
    for (boost::tokenizer<boost::char_separator<char> >::iterator tok_iter = tokens.begin();
            tok_iter != tokens.end(); ++tok_iter){
        if((*tok_iter).length() > 0){
            size_t pos = (*tok_iter).find('=');
            fix_map[(*tok_iter).substr(0, pos)] = (*tok_iter).substr(pos + 1);
        }
    }

    if(fix_map.find("35") == fix_map.end()){
        cout<<"key 35 not exist!!"<<endl;
        return false;
    }

    string k35 = fix_map["35"];
    if(k35 == "A"){
        str = "35=A49=emgMsgSvr56=551T112134=157=ADMIN52=20160721-07:05:02108=3098=0";
        msg_list.push_back(str);
        
    }
    else if(k35 == "0"){
        send_list.push_back("8=FIX.4.39=6535=049=emgMsgSvr56=551T112134=457=ADMIN52=20160721-07:05:2910=119");
        return true;
    }
    else if(k35 == "UL01"){
        str = "35=UL0249=emgMsgSvr56=551T112134=352=20160721-07:05:0280001=0380002=0280003=0280014=sinopacc80024=15045980004=1000553=123456789080011=123456789012345681024=3117=7000137=00001571=0000181014=0000181015=0000181016=0000175=2016072180031=00279";
        msg_list.push_back(str);
    }
    else if(k35 == "UP01"){
        string stkid      = fix_map["55"];
        string b_price    = fix_map["132"];
        string b_qty      = fix_map["134"];
        string s_price    = fix_map["133"];
        string s_qty      = fix_map["135"];
        string ticket_no  = fix_map["11"];
        string userdefined= fix_map["80014"];
        string str = "35=UP1049=emgMsgSvr56=551T112134=167652=20160714-08:11:4080001=0380002=0880003=1080014=0000000080024=16113880004=000011="+ticket_no+"81029=000000181039=0000.0081040="+b_qty+"81041="+b_price+"81028="+b_qty+"81032=000000281042=0000.0081043="+s_qty+"81044="+s_price+"81031="+s_qty+"";
        msg_list.push_back(str);

        cout<<"***************"<<endl;
        cout<<"user define:"<<userdefined<<endl;
        cout<<"ticket no:"<<ticket_no<<endl;
        cout<<"b_qty:["<<b_qty<<"] s_qty:["<<s_qty<<"]"<<endl;
        string key = stkid + ticket_no;
            cout<<"key:"<<key<<endl;
        if(g_fix_map.find(key) != g_fix_map.end()){
            vector<string>::iterator it = g_fix_map[key].begin();
            for(; it != g_fix_map[key].end() ; it++){
                cout<<*it<<endl;
                msg_list.push_back(*it);
            }
        }
        else{
            if(b_qty != "00000000"){
                str = "35=UC1749=emgMsgSvr56=551T112134=258652=20160722-08:51:2980001=0380002=0580003=1780014=        80024=16512980004=000055="+stkid+"54=144="+b_price+"38="+b_qty+"81023=150000";
                msg_list.push_back(str);
                cout<<"side 1:"<<b_qty<<endl;
            }
            if(s_qty != "00000000"){
                str = "35=UC1749=emgMsgSvr56=551T112134=258752=20160722-08:51:2980001=0380002=0580003=1780014=        80024=16512980004=000055="+stkid+"54=244="+s_price+"38="+s_qty+"81023=150000";
                msg_list.push_back(str);
                cout<<"side 2:"<<s_qty<<endl;
            }
        }
    }
    else if(k35 == "UP02"){
        string stk_id = fix_map["55"];
        string userdefine = fix_map["80014"];
        cout<<"userdefine:"<<userdefine<<endl;
        string key = stk_id + userdefine;
        cout<<"key:"<<key<<endl;

        vector<string>::iterator it = g_fix_map[key].begin();
        for(; it != g_fix_map[key].end() ; it++){
            cout<<*it<<endl;
            msg_list.push_back(*it);
        }
    }
    else{
        cout<<"unknow type!!!"<<endl;
    }
    for(list<string>::iterator i = msg_list.begin() ; i != msg_list.end(); i++){
        string send_msg = "8=FIX.4.39=" + boost::lexical_cast<string>((*i).length());
        send_msg += "" + *i;
        GetCheckSum(send_msg.c_str(), send_msg.length(), buf);
        send_list.push_back(send_msg + "10=" + buf +""); //buf = 152;
    }

    return true;
}

void pair_callback(int fd, short what, void *arg)
{
    uint8_t id[256];
    size_t id_size = 256;

    zmq::socket_t* socket = (zmq::socket_t*) arg;
    string pid = boost::lexical_cast<string>(getpid());

    list<string> send_list;
    int count = 0;
    while (1)
    {
        zmq::message_t message;
        socket->recv(&message);
        string text = std::string(static_cast<char*>(message.data()), message.size());

        if(text.find("8=FIX.4.3") != 0) continue;
        cout << "[RECV] #"<<pid<<"#" <<"["<< text <<"]"<< endl;

        socket->getsockopt(ZMQ_IDENTITY, id, &id_size);

        send_list.clear();
        message_parse(text, send_list);
        for(list<string>::iterator i = send_list.begin() ; i != send_list.end(); i++){
            socket->send(id, id_size, ZMQ_SNDMORE);
            cout<<"[Send size:"<<i->length()<<"] "<<*i<<endl;
            zmq::message_t reply (i->length());
            memcpy (reply.data (), i->c_str(), i->length());
            socket->send (reply);
            //sleep(1);
        }

        count++;
        /*
           if(count % 10000 == 0) {
           unsigned long ms = std::chrono::system_clock::now().time_since_epoch() /
           std::chrono::milliseconds(1);
           cout << "[RECV] #" << count << " at " << ms << ": \"" << text << "\"" << " " << message.size() << endl;
           }
           */
    }
    
}


void init_fix_map(string filename){
    ifstream infile(filename);
    string line, key, value;

    while(getline(infile, line)){
        size_t pos = line.find('');
        key = line.substr(0, pos);
        value = line.substr(pos +1);
        map<string, vector<string> >::iterator it;
        it = g_fix_map.find(key);
        if(it == g_fix_map.end()){
            vector<string> vec;
            vec.push_back(value);
            g_fix_map[key] = vec;
        }
        else{
            (it->second).push_back(value);
        }
    }
}

void task1(string msg){
    while(1){
        cout<<msg<<endl;
        sleep(1);
    }
}
void task2(string msg){
    while(1){
        cout<<msg<<endl;
        sleep(2);
    }
}

int main(int argc, char *argv[])
{
    string port = "32188";
    if(argc > 1){
        port = argv[1];
    }
    else{
    }
    struct event_base *evbase;
    struct event *ev;
    int pairfd;

    //thread t1(task1, "hello");
    //thread t2(task2, "hello 22");


    init_fix_map("fix.txt");

    zmq::context_t context;
    zmq::socket_t socket(context, ZMQ_STREAM);
    //socket.connect("tcp://127.0.0.1:" + port);
    //socket.bind("tcp://128.110.3.111:" + port);
    socket.bind("tcp://127.0.0.1:" + port);
    //socket.bind("tcp://*:" + port);
    cout << "connect to port:" <<port<<endl;

    pairfd = socket.getsockopt<int>(ZMQ_FD);

    evbase = event_base_new();
    ev = event_new(evbase, pairfd, EV_READ|EV_PERSIST, pair_callback, &socket);
    event_add(ev, NULL);

    event_base_dispatch(evbase);
    return 0;
}
