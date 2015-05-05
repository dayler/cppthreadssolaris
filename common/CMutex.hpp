/* 
 * File:   CMutex.hpp
 * Author: dayler
 *
 * Created on May 4, 2015, 6:38 PM
 */

#ifndef CMUTEX_HPP
#define	CMUTEX_HPP

using namespace std;

class CMutex {
public:
    CMutex(pthread_mutex_t* mutex);
    CMutex(const CMutex& orig);
    
    pthread_mutex_t* getMutex();
    void unlock();
    
    virtual ~CMutex();
private:
    pthread_mutex_t* mutex;
};

/* Constructors */
CMutex::CMutex(pthread_mutex_t* mutex)
{
    assert(mutex != NULL);
    this->mutex = mutex;
    // Lock mutex
    pthread_mutex_lock(mutex);
    
}

CMutex::CMutex(const CMutex& orig)
{
    // No op
}

/* Public methods */
pthread_mutex_t* CMutex::getMutex()
{
    return mutex;
}

void CMutex::unlock()
{
    // Unlock mutex
    pthread_mutex_unlock(mutex);
}

/* Destructor */
CMutex::~CMutex()
{
    // Unlock mutex
    unlock();
}

#endif	/* CMUTEX_HPP */
