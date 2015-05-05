/* 
 * File:   CSynch.hpp
 * Author: asalazar
 *
 * Created on May 5, 2015, 10:25 AM
 */

#ifndef CSYNCH_HPP
#define	CSYNCH_HPP

#include <stdlib.h>
#include <pthread.h>

#include "CMutex.hpp"

const long long FIX_TIME_MILLIS_TO_NANO = 1000000L;

class CSync
{
private:
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t syncVar = PTHREAD_COND_INITIALIZER;
    long timeout;
    struct timespec ts;
    struct timeval tp;

public:
    CSync();
    CSync(const CSync& orig);
    
    void doWait(long timeout);
    void doWait();
    void doNotifyAll();
    pthread_cond_t* getSyncCondVar();
    
    virtual ~CSync();
};

/* CSync */
/* Constructors */
CSync::CSync()
{
    // No op
}

CSync::CSync(const CSync& orig)
{
    // No op
}

/* Private methods */
/* Public methods */
void CSync::doWait()
{
    CMutex cmutex(&mutex); // lock
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

void CSync::doWait(long timeout)
{
    CMutex cmutex(&mutex); // lock
    
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

void CSync::doNotifyAll()
{
    CMutex cmutex(&mutex);
    // Condition of if statement has been met. 
    // Signal to free waiting thread by freeing the mutex.
    // Note: functionCount1() is now permitted to modify "count".
    pthread_cond_signal(&syncVar ); // notify all
}

pthread_cond_t* CSync::getSyncCondVar()
{
    return &syncVar;
}

/* Destructor */
CSync::~CSync()
{
    // Destroy sync condition.
    pthread_cond_destroy(&syncVar);
}

#endif	/* CSYNCH_HPP */
