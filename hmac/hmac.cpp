#include <openssl/sha.h>
#include <openssl/hmac.h>

#include <string>
#include <array>
#include<iostream>

unsigned char *mx_hmac_sha256(const void *key, int keylen,
        const unsigned char *data, int datalen,
        unsigned char *result, unsigned int *resultlen) {
    return HMAC(EVP_sha256(), key, keylen, data, datalen, result, resultlen);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cout<<argv[0]<<" key message"<<std::endl;
        exit(0);
    }
    std::string key = argv[1];
    std::string data = argv[2];
    unsigned char *result = NULL;
    unsigned int resultlen = -1;
    result = mx_hmac_sha256(reinterpret_cast<const void *>(key.c_str()), key.size(), reinterpret_cast<unsigned char const*>(data.c_str()), data.size(), result, &resultlen);
    for (unsigned int i = 0; i < resultlen; i++){
        printf("%02hhx", result[i]); // or just "%02X" if you are not using C11 or later
    }
    return 0;
}
