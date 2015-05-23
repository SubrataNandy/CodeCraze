/*** Bounded-buffer Producer-Consumer problem ***
 *** Multi-threaded solution ***/


#include <iostream>
#include <pthread.h>

using namespace std;

class DailyAvgStats {
  public:
    static const int BUFFSIZE = 10;  // Size of the buffer

    static const int PRODUCERS_COUNT = 5;  // Number of producers
    static const int MAX_ITEMS_PER_PRODUCER = 4; // Max number of items each producer can produce
    static const int TOTAL_ITEMS = 20; // Total number of items produced

    static const int CONSUMERS_COUNT = 6;  // Number of consumers
};

class Store { // TODO: Create singleton class

private:
  int _bufindex; // Current index where producer can put or consumer can get
  int _consumed; // Total items consumed so far

  pthread_mutex_t _buflock; // Lock of shared buffer 
  int *_produced; // Number of items produced so far by each porducer

  int *buffer; // The Shared Buffer

public:

  Store () : _bufindex(-1), _consumed(0)  {
    pthread_mutex_init(&_buflock, NULL);

    _produced = new int[DailyAvgStats::PRODUCERS_COUNT];
    for (int i=0; i<DailyAvgStats::PRODUCERS_COUNT; i++)
       _produced[i] = 0;

    buffer = new int[DailyAvgStats::BUFFSIZE];
  }

  void PutFromProducer(int which) {
    while (_produced[which] < DailyAvgStats::MAX_ITEMS_PER_PRODUCER) {
      pthread_mutex_lock(&_buflock);
        if (_bufindex < DailyAvgStats::BUFFSIZE-1) {
          buffer[++_bufindex] = which;
          cout << "Put at " << _bufindex << " by producer thread " << which << endl;
          _produced[which] ++;
        }
      pthread_mutex_unlock(&_buflock);
    }
  }
  void GetToConsumer(int which) {
    while (_consumed < DailyAvgStats::TOTAL_ITEMS) {
      pthread_mutex_lock(&_buflock);
        if (_bufindex >=0 ) {
          int item = buffer[_bufindex];
          cout << "Taken from " << _bufindex << " by consumer thread " << which << endl;
          _bufindex--;
          _consumed++;
        }
      pthread_mutex_unlock(&_buflock);
    }
  }
  ~Store() {
    pthread_mutex_destroy(&_buflock);
    delete [] _produced;
    delete [] buffer;
  }
};

struct thread_arg {
  int thread_id;
  Store* _store;
};


class Producer {
  public:
    static void* Produce(void* arg);
};

void* Producer::Produce(void* arg) {
  thread_arg *parg = (thread_arg*)arg;
  Store *store = parg->_store;
  if (store) {
    store->PutFromProducer(parg->thread_id);
  }
  else {
    cout << "Producer: " << parg->thread_id <<  " could not locate the store!!!" << endl;
  }
  pthread_exit((void*) 0);
}



class Consumer {
  public:
    static void* Consume(void* arg);
};

void* Consumer::Consume(void* arg) {
  thread_arg *carg = (thread_arg*)arg;
  Store *store = carg->_store;
  if (store) {
    store->GetToConsumer(carg->thread_id);
  }
  else {
    cout << "Consumer: " << carg->thread_id <<  " could not locate the store!!!" << endl;
  }
  pthread_exit((void*) 0);
}
class System {  // TODO: Create singleton class
  Store *singleton_store;

  public:

  System() {
    singleton_store = new Store;
  }

  void Simulate() {
    void *status;
    pthread_attr_t attr;


    pthread_t producer_threads[DailyAvgStats::PRODUCERS_COUNT];
    thread_arg* pargs[DailyAvgStats::PRODUCERS_COUNT];

    pthread_attr_init(&attr);

    for(int i=0; i<DailyAvgStats::PRODUCERS_COUNT; i++) {
      pargs[i] = new thread_arg;
      pargs[i]->thread_id = i;
      pargs[i]->_store = singleton_store;
      int rc = pthread_create(&producer_threads[i], &attr, Producer::Produce, pargs[i]);

      if (rc) {
        printf ("Unable to create producer thread %d\n", i);
      }
    }
    for(int i=0; i<DailyAvgStats::PRODUCERS_COUNT; i++) {
      int rc = pthread_join(producer_threads[i], &status);

      if (rc) {
        printf ("Unable to join producer thread %d\n", i);
      }
      delete pargs[i];
    }

    for(int i=0; i<DailyAvgStats::CONSUMERS_COUNT; i++) {
      int rc = pthread_join(consumer_threads[i], &status);

      if (rc) {
        printf ("Unable to join consumer thread %d\n", i);
      }
      delete cargs[i];
    }

    pthread_attr_destroy(&attr);
  }

  ~System() {
    delete singleton_store;
  }
};

int main() {

  System *sys = new System();

  sys->Simulate();

  delete sys;

  return 0;
}
