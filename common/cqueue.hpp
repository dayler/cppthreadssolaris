/* 
 * File:   cqueue.hpp
 *
 * NuevaTel PCS de Bolivia S.A. (C) 2010
 */

#ifndef CQUEUE_HPP
#define	CQUEUE_HPP

#include <queue>

#include "SMutex.hpp"
#include "SSynch.hpp"

template <class V>
class Queue
{
private:
    
    pthread_mutex_t mtx_pop;
    pthread_mutex_t mtx_push;
    pthread_mutex_t mtx_empty;
    /** The queue. */
    std::queue<V*> queue;
    
    /*
     * Synch monitor.
     */
    SSync* sync;

public:

    /**
     * Creates a new instance of Queue.
     */
    Queue()
    {
        pthread_mutex_init(&mtx_pop, NULL);
        pthread_mutex_init(&mtx_push, NULL);
        pthread_mutex_init(&mtx_empty, NULL);
        sync = new SSync();
    }
    
    virtual ~Queue()
    {
        pthread_mutex_destroy(&mtx_pop);
        pthread_mutex_destroy(&mtx_push);
        pthread_mutex_destroy(&mtx_empty);
        delete sync;
    }
    
    /**
     * Returns true if the queue is empty.
     * 
     * 
     * @return bool
     */
    bool empty()
    {
        SMutex smutex(&mtx_empty, true);
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
            SMutex cmutex(&mtx_push);
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
        SMutex cmutex(&mtx_pop);\
        while(queue.empty())
        {
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
