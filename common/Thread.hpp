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
#include <errno.h>

#include "Runnable.hpp"

using namespace std;

/**
 * TODO: GetState TERMINATED, etc..
 * 
 */
class Thread
{
private:
    pthread_t threadId;
    pthread_attr_t threadAttr;
    
    Runnable* runnable;
    
    bool detached;
    void* result;
    
    Thread(const Thread& thrd);
    
    virtual void* run();
    void setCompleted();
    static void* startThreadWithRunnable(void* pVoid);
    static void* startThreadWithoutRunnable(void* pVoid);
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
    // copy constructor.
}

/* Public Methods */

void Thread::start()
{
    // Initialize attribute object.
    int status = pthread_attr_init(&threadAttr);
    if (status != 0)
    {
        printError("On start when pthread_attr_init is called.", status, __FILE__, __LINE__);
        exit(status);
    }
    // PTHREAD_SCOPE_SYSTEM, meaning that the threads contend for CPU time with all processes
    // running on the machine.
    status = pthread_attr_setscope(&threadAttr, PTHREAD_SCOPE_SYSTEM);
    if (status != 0)
    {
        printError("start() failed on pthread_attr_setscope", status, __FILE__, __LINE__);
        exit(status);
    }
    
    if (!detached)
    {
        if (runnable == 0)
        {
            status = pthread_create(&threadId, &threadAttr, Thread::startThreadWithoutRunnable, (void*)this);
            if (status != 0)
            {
                stringstream ss;
                ss << "start() failed on pthread_create with out runnable and detached = "<<detached;
                printError(ss.str().c_str(), status, __FILE__, __LINE__);
                exit(status);
            }
        }
        else
        {
            int status = pthread_create(&threadId, &threadAttr, Thread::startThreadWithRunnable, (void*)this);
            if (status != 0)
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
        if (runnable == 0)
        {
            int status = pthread_create(&threadId, &threadAttr, Thread::startThreadWithoutRunnable, (void*)this);
            if (status != 0)
            {
                stringstream ss;
                ss << "start() failed on pthread_create with out runnable and detached = "<<detached;
                printError(ss.str().c_str(), status, __FILE__, __LINE__);
                exit(status);
            }
        }
        else
        {
            int status = pthread_create(&threadId, &threadAttr, Thread::startThreadWithRunnable, (void*)this);
            if (status != 0)
            {
                stringstream ss;
                ss << "start() failed on pthread_create with runnable and detached = "<<detached;
                printError(ss.str().c_str(), status, __FILE__, __LINE__);
                exit(status);
            }
        }
        
        status = pthread_attr_destroy(&threadAttr);
        if (status != 0)
        {
            stringstream ss;
            ss << "start() on pthread_attr_destroy threadId = " << threadId;
            printError(ss.str().c_str(), status, __FILE__, __LINE__);
            exit(status);
        }
    }

}

void* Thread::join()
{
    int status = pthread_join(threadId, NULL);
    if (ESRCH == status)
    {
        stringstream ss;
        ss<<"WARNING - "<<"STATUS:"<<status<<" FILE:"<<__FILE__<<" LINE:"<<__LINE__
          <<" MSG: Process was finished before execute join"<<endl;
        printf(ss.str().c_str());
        pthread_exit(NULL);
    }
    else if (status != 0)
    {
        stringstream ss;
        ss << "Failed on join for thread = "<<threadId;
        printError(ss.str().c_str(), status, __FILE__, __LINE__);
        pthread_exit(NULL);
    }
    
    return reinterpret_cast<void*>(status);
}

void Thread::init(Runnable* runnable, bool detached)
{
    this->runnable = runnable;
    this->detached = detached;
}

/* Private Methods */

void* Thread::run()
{
    cout<<"Executing NULL runnable..."<<endl;
    // No op
    return NULL;
}

void Thread::setCompleted()
{
    // Completion was handled by pthread_join()
}

void* Thread::startThreadWithRunnable(void* pVoid)
{
    // Start function when runnable is enabled.
    Thread* runnableThread = static_cast<Thread*>(pVoid);
    // Check not null.
    assert(runnableThread != NULL);
    // Execute run function.
    runnableThread->result = runnableThread->runnable->run();
    runnableThread->setCompleted();
    return runnableThread->result;
}

void* Thread::startThreadWithoutRunnable(void* pVoid)
{
    // Thread star the function when no runnable is invoked.
    Thread* thrd = static_cast<Thread*>(pVoid);
    // Check not null
    assert(thrd != 0);
    // Run itself implementation.
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
    // Destroying attr
    pthread_attr_destroy(&threadAttr);
}

#endif	/* THREAD_HPP */

