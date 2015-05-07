/* 
 * File:   main.cpp
 * Author: dayler
 *
 * Created on May 4, 2015, 4:31 PM
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <string>

#include "Utils.hpp"
#include "Runnable.hpp"
#include "SimpleRunnable.hpp"
#include "Thread.hpp"
#include "CMutex.hpp"
#include "CSynch.hpp"
#include "cqueue.hpp"

using namespace std;

//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

/* For safe condition variable usage, must use a boolean predicate and  */
/* a mutex with the condition.                                          */
int                 workToDo = 0;
//pthread_cond_t      cond  = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t     m = PTHREAD_MUTEX_INITIALIZER;

const int COUNT = 5;
int shared = 0;

class Item
{
public:
    string cstr;
    
    Item(string cstr)
    {
        this->cstr = cstr;
    }
    
    virtual ~Item()
    {
        // No op
    }
};

Queue<Item>* items;

#define NTHREADS                100
#define WAIT_TIME_SECONDS       15

// Produccer
class TH1 : public Runnable
{
public:
    int id;
    
    TH1(int id)
    {
        this->id = id;
    }
    
    ~TH1()
    {
        // No op
    }
    
    bool isRunning()
    {
        return shared < COUNT;
    }
    
    void* run()
    {
        while (isRunning())
        {
            printf("AAAA 1\n");
            string sshared = SSTR(shared);
            printf("AAAA 2\n");
            sleep(3);
            printf("AAAA 3\n");
            items->push(new Item(sshared));
            printf("THID:%d Pushed %s\n", id, sshared.c_str());
            shared++;
        }
        return reinterpret_cast<void*>(id);
    }
};

// Consummer
class TH2 : public Runnable
{
public:
    int id;
    
    TH2(int id)
    {
        this->id = id;
    }
    
    ~TH2()
    {
        // No op
    }
    
    bool isRunning()
    {
        // If shred is less than MAX or items is not empty.
        return shared < COUNT || !items->empty();
    }
    
    void* run()
    {
        while (isRunning())
        {
            printf("BBBB\n");
            sleep(1);
            Item* val = items->waitAndPop();
            printf("THID:%d q->waitAndPop() = %s\n", id, val->cstr.c_str());
            delete val;
        }
        return reinterpret_cast<void*>(id);
    }
};

int main()
{
    cout<<"Start main..."<<endl;
    items = new Queue<Item>();
    
    printf("Init runnables.\n");
    Runnable* r1 = new TH1(111);
    Runnable* r2 = new TH2(222);
//    Runnable* r3 = new TH2(333);
    
    Thread* t1 = new Thread(r1, false);
    Thread* t2 = new Thread(r2, false);
//    Thread* t3 = new Thread(r3, false);
    
    // Start thread
    printf("Starting threads...\n");
    t2->start(); // Consumer
    t1->start(); // Producer
//    t3->start(); // Consumer
    printf("Was started threads joining ...\n");
    
    // Joining threads
    long res1 = reinterpret_cast<long>(t1->join()); // Producer
    cout<<"Result t1 = "<<res1<<endl;
    long res2 = reinterpret_cast<long>(t2->join()); // Consumer
    cout<<"Result t2 = "<<res2<<endl;
//    long res3 = reinterpret_cast<long>(t3->join()); // Consumer
//    cout<<"Result t2 = "<<res3<<endl;
    
    // delete
    delete r1;
    delete r2;
//    delete r3;
    delete t1;
    delete t2;
//    delete t3;
    delete items;
    
    cout<<"Finish well main..."<<endl;
    return 0;
}

/**
 * Threads basic
 */
//void* functionC(void* arg)
//{
//   CMutex m(&mutex1);
//   // pthread_mutex_lock( &mutex1 );
//   counter++;
//   printf("Counter value: %d\n",counter);
//   // pthread_mutex_unlock( &mutex1 );
//}
//
//int main()
//{
//   int rc1, rc2;
//   pthread_t thread1, thread2;
//
//   // Create independent threads each of which will execute functionC /
//
//   if( (rc1=pthread_create( &thread1, NULL, &functionC, NULL)) )
//   {
//      printf("Thread creation failed: %d\n", rc1);
//   }
//
//   if( (rc2=pthread_create( &thread2, NULL, &functionC, NULL)) )
//   {
//      printf("Thread creation failed: %d\n", rc2);
//   }
//
//   // Wait till threads are complete before main continues. Unless we  /
//   // wait we run the risk of executing an exit which will terminate   /
//   // the process and all threads before the threads have completed.   /
//
//   pthread_join( thread1, NULL);
//   pthread_join( thread2, NULL); 
//
//   exit(0);
//}

//int main() {
//    cout<<"Begin main test class..."<<endl;
//    
//    Runnable* r1 = new SimpleRunnable(1, 3);
//    Runnable* r2 = new SimpleRunnable(2, 8);
//    
//    Thread* t1 = new Thread(r1, false);
//    Thread* t2 = new Thread(r2, false);
//    
//    // Start threads
//    t1->start();
//    t2->start();
//    
//    // Join threads
//    long res1 = reinterpret_cast<long>(t1->join());
//    long res2 = reinterpret_cast<long>(t2->join());
//    
//    cout<<"Result t1 = "<<res1<<endl;
//    cout<<"Result t2 = "<<res2<<endl;
//    
//    // Delete objects
//    delete r1;
//    delete t1;
//    
//    delete r2;
//    delete t2;
//    
//    cout<<"Finish main test class..."<<endl;
//    return 0;
//}

