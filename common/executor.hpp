/* 
 * File:   executor.hpp
 *
 * NuevaTel PCS de Bolivia S.A. (C) 2010
 */

#ifndef _EXECUTOR_HPP
#define	_EXECUTOR_HPP

#include "Thread.hpp"
#include "worker.hpp"

#include <vector>

/**
 * <p>The Executor class.</p>
 *
 * @author  Eduardo Marin
 * @version 1.0, 05-19-2010
 */
class Executor {

    static int DELETE_TERMINATED_PERIOD;

    /** The threadVector. */
    std::vector<Thread*> threadVector;
    unsigned int threadIdx;

    /** The deleteTimer. */
    Worker *deleteTimer;
    Runnable *deleteTimerTask;

    /** The executorMutex. */
//    boost::mutex executorMutex;
    pthread_mutex_t mtx;

public:

    /**
     * Creates a new instance of Executor.
     */
    Executor() {
        pthread_mutex_init(&mtx, NULL);
        
        deleteTimer=new Worker("delete-timmer");
        deleteTimerTask=new DeleteTimerTask(this, 0);
        deleteTimer->scheduleAtFixedRate(deleteTimerTask, DELETE_TERMINATED_PERIOD, DELETE_TERMINATED_PERIOD);
        threadIdx=0;
    }

    virtual ~Executor() {
        delete deleteTimer;
        delete deleteTimerTask;
        for(unsigned int i=0; i < threadVector.size(); i++) delete threadVector[i];
        
        pthread_mutex_destroy(&mtx);
    }

    /**
     * Submits a new thread for execution. The thread pointer will be deleted 
     * after execution.
     * @param *thread Thread
     */
//    void submit(Thread *thread) // Replace by runnable
    void submit(Runnable* run) // Replace by runnable
    {
        SMutex smutex(&mtx);
        Thread* th = new Thread(run, false);
        while(threadIdx < threadVector.size())
        {
            Thread *tmpThread=threadVector[threadIdx];
            if (Thread::TERMINATED == th->getState())
            //if(tmpThread->getState()==Thread::TERMINATED)
            {
                delete tmpThread;
//                threadVector[threadIdx]=thread;
                threadVector[threadIdx]=th;
                if(threadIdx < threadVector.size() - 1)
                {
                    threadIdx++;
                }
                else
                {
                    threadIdx=0;
                }
                th->start();
                return;
            }
            else
            {
                if(threadIdx < threadVector.size() - 1)
                {
                    threadIdx++;
                }
                else
                {
                    break;
                }
            }
        }
        
        threadVector.push_back(th);
        threadIdx=0;
        th->start();
    }

private:

    /**
     * Deletes all terminated threads.
     */
    void deleteTerminated() {
        SMutex smutex(&mtx);
        std::vector<Thread*>::iterator iter;
        iter=threadVector.begin();
        while(iter!=threadVector.end()) {
            Thread *thread=*iter;
            if(thread->getState()==Thread::TERMINATED) {
                delete thread;
                iter=threadVector.erase(iter);
            }
            else iter++;
        }
        threadIdx=0;
    }

    class DeleteTimerTask : public Runnable {

    private:
        /** The executor. */
        Executor *executor;
        
        int id;

    public:

        /**
         * Creates a new instance of DeleteTimerTask.
         * @param *executor Executor
         */
        DeleteTimerTask(Executor *executor, int id) {
            this->executor=executor;
            this->id = id;
        }

    private:
        void* run() {
            executor->deleteTerminated();
            return reinterpret_cast<void*>(id);
        }
    };
};

int Executor::DELETE_TERMINATED_PERIOD=60000;

#endif	/* _EXECUTOR_HPP */
