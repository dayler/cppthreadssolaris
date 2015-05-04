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
    virtual ~CMutex();
private:
    pthread_mutex_t* mutex = NULL;
};

/* Constructors */
CMutex::CMutex(pthread_mutex_t* mutex)
{
    assert(mutex != NULL);
    this->mutex = mutex;
    // Lock mutex
    pthread_mutex_lock(mutex);
    
}

/* Private methods */

/* Destructor */
CMutex::~CMutex()
{
    if (mutex != NULL)
    {
        // Unlock mutex
        pthread_mutex_unlock(mutex);
    }
}

#endif	/* CMUTEX_HPP */

