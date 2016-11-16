#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <hiredis.h>
#include <boost/lexical_cast.hpp>

using namespace std;

void redis_set(redisContext* c, string& key, string& value){
    /* Set a key */
    redisReply *reply;
    reply = (redisReply*)redisCommand(c,"SET %s %s", key.c_str(), value.c_str());
    //printf("SET: %s\n", reply->str);
    freeReplyObject(reply);
}

void redis_get(redisContext* c, string& key, string& val){
    /* Set a key */
    redisReply *reply;
    reply = (redisReply*)redisCommand(c,"GET %s", key.c_str());
    val = reply->str;
    //printf("SET: %s\n", reply->str);
    freeReplyObject(reply);
}
int main(int argc, char **argv) {
     srand (time(NULL));

    if(argc != 3){
        cout<<"invalid parameter!!"<<endl;
        return 0;
    }
    int counter = 0;
    int type = atoi(argv[1]);

    string key, val;
    if(type == 3){
        key = argv[2];
    }
    else{
        counter = atoi(argv[2]);
    }

    cout<<"type:"<<type<<endl;
    cout<<"counter:"<<counter<<endl;

    redisContext *c;
    const char *hostname = "127.0.0.1";
    int port = 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = (redisContext *)redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    /* PING server */

    redisReply *reply;
    if(type == 3){
        redis_get(c, key, val);
        cout<<"ret:"<<val<<endl;
    }
    else if(type == 1){
        time_t now = time(NULL);
        for(int i = 0; i < counter ; i++){
            key = "key_" + boost::lexical_cast<string>(i);
            val = "val_" + boost::lexical_cast<string>(i) +"_" + boost::lexical_cast<string>(now);
            reply = (redisReply*)redisCommand(c,"SET %s %s", key.c_str(), val.c_str());
        }
    }
    else if(type == 2){
        int k;
        for(int i = 0; i < counter ; i++){
            k = rand() % counter;
             
            key = "key_" + boost::lexical_cast<string>(k);
            //redis_get(c, key, val);
            reply = (redisReply*)redisCommand(c,"GET %s", key.c_str());
            //printf("SET: %s\n", reply->str);
        }
        val = reply->str;
        cout<<key<<":"<<val<<endl;
    }
    freeReplyObject(reply);


    /* Set a key using binary safe API */

    /* Disconnects and frees the context */
    redisFree(c);

    return 0;
}
