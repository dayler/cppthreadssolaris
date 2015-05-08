/*
 * File:   timer.hpp
 *
 * NuevaTel PCS de Bolivia S.A. (C) 2010
 */

#ifndef _WORKER_HPP
#define	_WORKER_HPP

#include "Thread.hpp"
#include "SSynch.hpp"

#include <unistd.h>

using namespace std;

/**
 * <p>The Timer class.</p>
 *
 * @author  Eduardo Marin
 * @version 1.0, 04-16-2010
 */
class Worker : public Thread {

    /** The task. */
    Runnable *task;

    /* private variables */
    long delay;
    long period;
    
    volatile bool cancelled;
    
    SSync* sync;
    pthread_mutex_t mtx;
    
    string name;

public:

    /**
     * Creates an unscheduled timer.
     */
    Worker(char* name) : Thread()
    {
        this->name = name;
        cancelled=false;
        sync = new SSync();
        pthread_mutex_init(&mtx, NULL);
    }

    virtual ~Worker()
    {
        cancel();
        pthread_mutex_destroy(&mtx);
        delete sync;
    }

    string getName()
    {
        return name;
    }
    
    /**
     * Cancels the timer.
     */
    void cancel()
    {
        {
            SMutex cmutex(&mtx);
            cancelled = true;
        }
        sync->doNotifyAll();
        join();
    }

    /**
     * Schedules the specified task for execution after the specified delay.
     * @param *task TimerTask
     * @param &delay const long
     */
    void scheduleAt(Runnable* task, const long &delay)
    {
        scheduleAtFixedRate(task, delay, 0);
    }

    /**
     * Schedules the specified task for repeated fixed-rate execution, beginning after the specified delay.
     * @param *task TimerTask
     * @param &delay const long
     * @param &period const long
     */
    void scheduleAtFixedRate(Runnable* task, const long &delay, const long &period)
    {
        cancelled = false;
        this->task = task;
        this->delay = delay;
        this->period = period;
        start();
    }

private:

    void* run()
    {
        if (delay > 0L)
        {
            // sleep thread at delay
            sync->doWait(delay);
        }
        
        if(!cancelled)
        {
            task->run();
            if(period > 0)
            {
                while(true)
                {
                    sync->doWait(period);
                    if (!cancelled)
                    {
                        task->run();
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        
        return (void*)name.c_str();
    }
};

#endif	/* _TIMER_HPP */
