#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
#include <semaphore>
#include "parser.h"



class QueueMutex{
    private:
        std::queue<Pair> queue;
        std::mutex mtx;
        std::counting_semaphore<> sem;
    public:
        QueueMutex() : sem(0) {};

        void push(Pair data){
            mtx.lock();
            queue.push(data);
            mtx.unlock();
            sem.release();
        }

        Pair pop(){
            sem.acquire();
            mtx.lock();
            Pair popped_item = queue.front();
            queue.pop();
            mtx.unlock();
            return popped_item;
        }
        

};

#endif
