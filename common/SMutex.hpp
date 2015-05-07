/* 
 * File:   CMutex.hpp
 * Author: dayler
 *
 * Created on May 4, 2015, 6:38 PM
 */

#ifndef SMUTEX_HPP
#define	SMUTEX_HPP

using namespace std;

class SMutex {
public:
    SMutex(pthread_mutex_t* mutex, bool tryLock);
    SMutex(pthread_mutex_t* mutex);
    SMutex(const SMutex& orig);
    
    pthread_mutex_t* getMutex();
    void unlock();
    
    bool isLocked();
    int getRetCode();
    
    virtual ~SMutex();
private:
    pthread_mutex_t* mutex;
    bool locked;
    int retCode;
};

/* Constructors */
SMutex::SMutex(pthread_mutex_t* mutex, bool tryLock)
{
    assert(mutex != NULL);
    this->mutex = mutex;
    if (tryLock)
    {
        retCode = pthread_mutex_trylock(mutex);
    }
    else
    {
        retCode = pthread_mutex_lock(mutex);
    }
    locked = retCode == 0;
}

SMutex::SMutex(pthread_mutex_t* mutex)
{
    assert(mutex != NULL);
    this->mutex = mutex;
    // Lock mutex
    retCode = pthread_mutex_lock(mutex);
    locked = retCode == 0;
}

SMutex::SMutex(const SMutex& orig)
{
    // No op
}

/* Public methods */
int SMutex::getRetCode()
{
    return retCode;
}

bool SMutex::isLocked()
{
    return locked;
}

pthread_mutex_t* SMutex::getMutex()
{
    return mutex;
}

void SMutex::unlock()
{
    // Unlock mutex
    pthread_mutex_unlock(mutex);
}

/* Destructor */
SMutex::~SMutex()
{
    // Unlock mutex
    unlock();
}

#endif	/* CMUTEX_HPP */
