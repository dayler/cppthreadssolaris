/* 
 * File:   CSynch.hpp
 * Author: asalazar
 *
 * Created on May 5, 2015, 10:25 AM
 */

#ifndef SSYNC_HPP
#define	SSYNC_HPP

#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#include "SMutex.hpp"

const long long FIX_TIME_MILLIS_TO_NANO = 1000000UL;

class SSync
{
private:
    pthread_mutex_t mutex;
    pthread_cond_t syncVar;
    
    long timeout;
    struct timespec ts;
    struct timeval tp;
    
public:
    SSync();
    SSync(const SSync& orig);
    
    void doWait(long timeout);
    void doWait();
    void doNotifyAll();
    pthread_cond_t* getSyncCondVar();
    pthread_mutex_t* getMutex();
    
    virtual ~SSync();
};

/* CSync */
/* Constructors */
SSync::SSync()
{
    // Initialize sync and mutex
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&syncVar, NULL);
}

SSync::SSync(const SSync& orig)
{
    // No op
}

/* Private methods */

/* Public methods */
void SSync::doWait()
{
    SMutex cmutex(&mutex); // lock
    int rc = pthread_cond_wait(&syncVar, &mutex);
    if (rc != 0)
    {
        printf("Failed on pthread_cond_wait RC=%d\n", rc);
        cmutex.unlock();
        pthread_exit(NULL);
    }
    else
    {
        cmutex.unlock();
    }
}

void SSync::doWait(long timeout)
{
    SMutex cmutex(&mutex); // lock
    
    this->timeout = timeout;
    int rc = gettimeofday(&tp, NULL);
    if (rc != 0)
    {
        printf("Failed on gettimeofday(&tp, NULL);\n");
        pthread_exit(NULL);
    }
    
    /* Convert from timeval to timespec */
    ts.tv_sec  = tp.tv_sec;
    ts.tv_nsec = tp.tv_usec * 1000;
    ts.tv_nsec += timeout * 1000000UL;
    
    // Wait while functionCount2() operates on count
    // mutex unlocked if condition varialbe in functionCount2() signaled.
    rc = pthread_cond_timedwait(&syncVar, &mutex, &ts);
    if (rc == ETIMEDOUT)
    {
        printf("ETIMEDOUT on pthread_cond_timedwait RC = %d\n", rc);
        cmutex.unlock();
        //pthread_exit(NULL);
    }
    else
    {
        cmutex.unlock();
    }
}

void SSync::doNotifyAll()
{
    SMutex cmutex(&mutex);
    // Condition of if statement has been met. 
    // Signal to free waiting thread by freeing the mutex.
    // Note: functionCount1() is now permitted to modify "count".
    // pthread_cond_signal(&syncVar ); // notify all
    pthread_cond_broadcast(&syncVar);
}

pthread_cond_t* SSync::getSyncCondVar()
{
    return &syncVar;
}

pthread_mutex_t* SSync::getMutex()
{
    return &mutex;
}

/* Destructor */
SSync::~SSync()
{
    // Destroy mutex
    pthread_mutex_destroy(&mutex);
    // Destroy sync condition.
    pthread_cond_destroy(&syncVar);
}

#endif	/* CSYNCH_HPP */
