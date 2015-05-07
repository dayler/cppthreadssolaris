/* 
 * File:   cqueue.hpp
 *
 * NuevaTel PCS de Bolivia S.A. (C) 2010
 */

#ifndef CQUEUE_HPP
#define	CQUEUE_HPP

#include <queue>

#include "CMutex.hpp"
#include "CSynch.hpp"

template <class V>
class Queue
{
private:
    
    pthread_mutex_t mtx;
    
    /** The queue. */
    std::queue<V*> queue;
    
    /*
     * Synch monitor.
     */
    CSync* sync;

public:

    /**
     * Creates a new instance of Queue.
     */
    Queue()
    {
        pthread_mutex_init(&mtx, NULL);
        sync = new CSync();
    }
    
    virtual ~Queue()
    {
        pthread_mutex_destroy(&mtx);
        delete sync;
    }
    
    /**
     * Returns true if the queue is empty.
     * @return bool
     */
    bool empty()
    {
        CMutex cmutex(&mtx);
        return queue.empty();
    }
    
    volatile int size()
    {
        return static_cast<int>(queue.size());
    }

    /**
     * Pushes an element.
     * @param *v V
     */
    void push(V *value)
    {
        {
            CMutex cmutex(&mtx);
            queue.push(value);
        }
        sync->doNotifyAll();
    }

    /**
     * Returns the front element.
     * @return *V
     */
    V* waitAndPop()
    {
        CMutex cmutex(&mtx);
        while(queue.empty())
        {
            printf("cqueue#sync->doWait()\n");
            // Block thread to await new items.
            sync->doWait();
        }

        // Get front element.
        V *v = queue.front();
        // Remove first element.
        queue.pop();
        return v;
    }
};

#endif	/* CQUEUE_HPP */
