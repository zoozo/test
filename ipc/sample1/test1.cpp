#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

#include <boost/interprocess/sync/scoped_lock.hpp>
//#include "doc_anonymous_mutex_shared_data.hpp"
#include <iostream>
#include <cstdio>
using namespace std;

using namespace boost::interprocess;
struct shared_memory_log
{
   enum { NumItems = 10 };
   enum { LineSize = 10 };

   shared_memory_log()
      :  current_line(0)
      ,  end_a(false)
      ,  end_b(false)
   {}

   //Mutex to protect access to the queue
   boost::interprocess::interprocess_mutex mutex;

   //Items to fill
   char   items[NumItems][LineSize];
   int    current_line;
   bool   end_a;
   bool   end_b;
};

int main ()
{
   try{
      //Remove shared memory on construction and destruction
      struct shm_remove
      {
         shm_remove() { shared_memory_object::remove("MySharedMemory"); }
         ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
      } remover;

      //Create a shared memory object.
      shared_memory_object shm
         (create_only               //only create
         ,"MySharedMemory"          //name
         ,read_write   //read-write mode
         );

      //Set size
      shm.truncate(sizeof(shared_memory_log));

      //Map the whole shared memory in this process
      mapped_region region
         (shm                       //What to map
         ,read_write   //Map it as read-write
         );

      //Get the address of the mapped region
      void * addr       = region.get_address();

      //Construct the shared structure in memory
      shared_memory_log * data = new (addr) shared_memory_log;

      //Write some logs
      for(int i = 0; i < shared_memory_log::NumItems; ++i){
         //Lock the mutex
         scoped_lock<interprocess_mutex> lock(data->mutex);
         std::printf(data->items[(data->current_line++) % shared_memory_log::NumItems]
                  ,"%s__%d", "pr---ss_a", i);
         sleep(1);
         cout<<i<<endl;
         if(i == (shared_memory_log::NumItems-1))
            data->end_a = true;
         //Mutex is released here
      }

      //Wait until the other process ends
      cout<<"aa:"<<__LINE__<<endl;
      while(1){
         scoped_lock<interprocess_mutex> lock(data->mutex);
         if(data->end_b)
            break;
      }
   }
   catch(interprocess_exception &ex){
      std::cout << ex.what() << std::endl;
      return 1;
   }
   return 0;
}
