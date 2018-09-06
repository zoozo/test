#include <sparsehash/dense_hash_map>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <unordered_map>
#include <map>

double get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec/1000000.0 ;
}

void test_google_hash(int n){
  google::dense_hash_map<std::string, std::string> dmap;
  dmap.set_empty_key("");

  double t1 =  get_time();
  for (int i = 0 ; i < n ; i++){
      //std::cout<<i<<std::endl;
      dmap.insert(google::dense_hash_map<std::string, std::string>::value_type(std::to_string(i), std::to_string(i+5)));
  }
  double t2 =  get_time();
  printf("insert google hash:%f\n", t2 - t1);

  double t3 =  get_time();
  std::string value;
  for (int i = 0 ; i < n ; i++){
       auto v = dmap.find(std::to_string(rand() % n));
  }
  double t4 =  get_time();
  printf("find google hash:%f\n", t4-t3);
  
}
void test_map(int n){
  std::map<std::string, std::string> dmap;

  double t1 =  get_time();
  for (int i = 0 ; i < n ; i++){
      //std::cout<<i<<std::endl;
      auto v = dmap.insert(std::map<std::string, std::string>::value_type(std::to_string(i), std::to_string(i+5)));
  }
  double t2 =  get_time();

  printf("insert map:%f\n", t2-t1);
  double t3 =  get_time();
  
  std::string value;
  for (int i = 0 ; i < n ; i++){
       auto v = dmap.find(std::to_string(rand() % n));
  }
  double t4 =  get_time();
  printf("find map:%f\n", t4-t3);
  
}
void test_unordered_map(int n){
  std::unordered_map<std::string, std::string> dmap;

  double t1 =  get_time();
  for (int i = 0 ; i < n ; i++){
      dmap.insert(std::unordered_map<std::string, std::string>::value_type(std::to_string(i), std::to_string(i+5)));
  }
  double t2 = get_time();
  printf("insert unordered_map:%f\n", t2 - t1);
  
  double t3 =  get_time();
  std::string value;
  for (int i = 0 ; i < n ; i++){
       dmap.find(std::to_string(rand() % n));
  }
  double t4 =  get_time();
  printf("find unordered map:%f\n", t4-t3);
}
int main(int argc, char** argv) {
    srand (time(NULL));

    if(argc != 2){
        std::cout<<"argc="<<argc<<std::endl;
        return 0;
    }
    int n = atoi(argv[1]);
    test_map(n);
    test_unordered_map(n);
    test_google_hash(n);

  

      
      std::cout<<"done"<<std::endl;
  return 0;
}
