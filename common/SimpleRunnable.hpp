/* 
 * File:   SimpleRunnable.hpp
 * Author: asalazar
 *
 * Created on April 29, 2015, 5:24 PM
 */

#ifndef SIMPLERUNNABLE_HPP
#define	SIMPLERUNNABLE_HPP

#include "Runnable.hpp"

using namespace std;

class SimpleRunnable : public Runnable
{
private:
    int myId;
public:
    SimpleRunnable();
    SimpleRunnable(int myId);
    
    void* run();
    
    virtual ~SimpleRunnable();
};

/* Constructors */
SimpleRunnable::SimpleRunnable() : Runnable()
{
    myId = -1;
}

SimpleRunnable::SimpleRunnable(int myId) : Runnable()
{
    this->myId = myId;
}

/* Public methods */
void* SimpleRunnable::run()
{
    Runnable::run();
    cout<<"Thread myId:"<<myId<<" is running..."<<endl;
    // Return myId if run() was succeeded.
    return reinterpret_cast<void*>(myId);
}
/* Private methods */

/* Destructor */
SimpleRunnable::~SimpleRunnable() {
}

#endif	/* SIMPLERUNNABLE_HPP */

