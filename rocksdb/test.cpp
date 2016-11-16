#include <cstdio>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/options.h"

using namespace rocksdb;
using namespace std;

std::string kDBPath = "/tmp/rocksdb_simple_example";

int main(int argc, char** argv) {
    srand (time(NULL));

    if(argc != 3){
        cout<<"invalid parameter!!"<<endl;
        return 0;
    }
    string key, val;
    int type = atoi(argv[1]);
    int counter = 0;

    if(type == 3){
        key = argv[2];
    }
    else{
        counter = atoi(argv[2]);
    }

    cout<<"type:"<<type<<endl;
    cout<<"counter:"<<counter<<endl;


  DB* db;
  Options options;
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  options.IncreaseParallelism();
  options.OptimizeLevelStyleCompaction();
  // create the DB if it's not already present
  options.create_if_missing = true;

  // open DB
  Status s = DB::Open(options, kDBPath, &db);
  assert(s.ok());


  if(type == 3){
      s = db->Get(ReadOptions(), key, &val);
      cout<<"ret:"<<val<<endl;
  }
  else if(type == 1){
      time_t now = time(NULL);
      for(int i = 0; i < counter ; i++){
          key = "key_" + boost::lexical_cast<string>(i);
          val = "val_" + boost::lexical_cast<string>(i)+"_" +boost::lexical_cast<string>(now);

          s = db->Put(WriteOptions(), key, val);
      }
  }
  else if(type == 2){
      int k;
      for(int i = 0; i < counter ; i++){
          k = rand() % counter;

          key = "key_" + boost::lexical_cast<string>(k);

          s = db->Get(ReadOptions(), key, &val);
      }
      cout<<key<<":"<<val<<endl;
  }
    delete db;
    return 0;
}
