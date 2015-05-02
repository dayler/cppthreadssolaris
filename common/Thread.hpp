/* 
 * File:   Thread.hpp
 * Author: asalazar
 *
 * Created on April 29, 2015, 4:05 PM
 */

#ifndef THREAD_HPP
#define	THREAD_HPP

#include<iostream>
#include<sstream>
#include<stdio.h>
#include<cstdlib>
#include<pthread.h>
#include <unistd.h>
#include<assert.h>

#include "Runnable.hpp"

using namespace std;

class Thread
{
private:
    pthread_t threadId;
    pthread_attr_t threadAttr;
    
    Runnable*runnable;
    bool detached;
    void* result;
    
    Thread(const Thread& thrd);
    
    virtual void* run();
    void setCompleted();
    static void* startThreadRunnable(void* pVoid);
    static void* startThread(void* pVoid);
    void printError(const char* msg, int status, char* fileName, int lineNumber);
    
    void init(Runnable* runnable, bool detached);
    
public:
    Thread();
    Thread(Runnable* runnable, bool detached);
    Thread(bool detached);
    void start();
    void* join();
    virtual ~Thread();
    
};

/* Constructors */

Thread::Thread()
{
    init(NULL, false);
}

Thread::Thread(bool detached)
{
    init(NULL, detached);
}

Thread::Thread(Runnable* runnable, bool detached)
{
    init(runnable, detached);
}

Thread::Thread(const Thread& thrd)
{
    // For = operator
}

/* Public Methods */

void Thread::start()
{
    // Initialize attribute object.
    int status = pthread_attr_init(&threadAttr);
    if (status != NULL)
    {
        printError("On start when pthread_attr_init is called.", status, __FILE__, __LINE__);
        exit(status);
    }
    // PTHREAD_SCOPE_SYSTEM, meaning that the threads contend for CPU time with all processes
    // running on the machine.
    status = pthread_attr_setscope(&threadAttr, PTHREAD_SCOPE_SYSTEM);
    if (status != NULL)
    {
        printError("start() failed on pthread_attr_setscope", status, __FILE__, __LINE__);
        exit(status);
    }
    
    if (!detached)
    {
        if (runnable == NULL)
        {
            int status = pthread_create(&threadId, &threadAttr, Thread::startThread, (void*)this);
            if (status != NULL)
            {
                stringstream ss;
                ss << "start() failed on pthread_create with out runnable and detached = "<<detached;
                printError(ss.str().c_str(), status, __FILE__, __LINE__);
                exit(status);
            }
        }
        else
        {
            int status = pthread_create(&threadId, &threadAttr, Thread::startThreadRunnable, (void*)this);
            if (status != NULL)
            {
                stringstream ss;
                ss << "start() failed on pthread_create with runnable and detached="<<detached;
                printError(ss.str().c_str(), status, __FILE__, __LINE__);
                exit(status);
            }
        }
    }
    else
    {
        // set the detached state attribute to detached.
        status = pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
        if (runnable == NULL)
        {
            int status = pthread_create(&threadId, &threadAttr, Thread::startThread, (void*)this);
            if (status != NULL)
            {
                stringstream ss;
                ss << "start() failed on pthread_create with out runnable and detached = "<<detached;
                printError(ss.str().c_str(), status, __FILE__, __LINE__);
                exit(status);
            }
        }
        else
        {
            int status = pthread_create(&threadId, &threadAttr, Thread::startThreadRunnable, (void*)this);
//            if (status != NULL)
//            {
//                stringstream ss;
//                ss << "start() failed on pthread_create with runnable and detached = "<<detached;
//                printError(ss.str().c_str(), status, __FILE__, __LINE__);
//                exit(status);
//            }
            
        }
        
        status = pthread_attr_destroy(&threadAttr);
//        if (status != NULL)
//        {
//            stringstream ss;
//            ss << "start() on pthread_attr_destroy threadId = " << threadId;
//            printError(ss.str().c_str(), status, __FILE__, __LINE__);
//            exit(status);
//        }
    }

}

void* Thread::join()
{
    int status = pthread_join(threadId, NULL);
//    if (status != NULL)
//    {
//        stringstream ss;
//        ss << "Failed on join for thread = "<<threadId;
//        printError(ss.str().c_str(), status, __FILE__, __LINE__);
//        exit(status);
//    }
}

void Thread::init(Runnable* runnable, bool detached)
{
    this->runnable = runnable;
    this->detached = detached;
}

/* Private Methods */

void* Thread::run()
{
    // No op
    return NULL;
}

void Thread::setCompleted()
{
    // Completion was handled by pthread_join()
}

void* Thread::startThreadRunnable(void* pVoid)
{
    // Start function when runnable is enabled.
    Thread* runnableThread = static_cast<Thread*>(pVoid);
    assert(runnableThread != NULL);
    runnableThread->result = runnableThread->run();
    runnableThread->setCompleted();
    return runnableThread->result;
}

void* Thread::startThread(void* pVoid)
{
    // Thread star the function when no runnable is invoked.
    Thread* thrd = static_cast<Thread*>(pVoid);
    assert(thrd != NULL);
    thrd->result = thrd->run();
    thrd->setCompleted();
    return thrd->result;
}

void Thread::printError(const char* msg, int status, char* fileName, int lineNumber)
{
    stringstream ss;
    ss<<"ERROR - "<<"STATUS:"<<status<<" FILE:"<<fileName<<" LINE:"<<lineNumber<<" MSG:"<<msg<<endl;
    printf(ss.str().c_str());
}

/* Destructor */
Thread::~Thread()
{
    // 
}

#endif	/* THREAD_HPP */
