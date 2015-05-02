/* 
 * File:   Runnable.hpp
 * Author: asalazar
 *
 * Created on April 29, 2015, 3:43 PM
 */

#ifndef RUNNABLE_HPP
#define	RUNNABLE_HPP

class Runnable
{
public:
    Runnable();
    virtual void* run();
    virtual ~Runnable();
private:

};

Runnable::Runnable()
{
    // No op
}

void* Runnable::run()
{
    // No op.
    return NULL;
}

Runnable::~Runnable()
{
    // No op
}

#endif	/* RUNNABLE_HPP */

