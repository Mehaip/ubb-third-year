#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
#include "parser.h"
#include <condition_variable>


class QueueMutex{
    private:
        std::queue<Pair> queue;
        std::mutex mtx;
        std::condition_variable cv_not_full;
        std::condition_variable cv_not_empty;
        int MAX = 100;
    public:
        QueueMutex(){}

        void push(Pair data){
            std::unique_lock<std::mutex> lock(mtx);
            cv_not_full.wait(lock, [this]{
                return queue.size() < MAX;
            });
            queue.push(data);
            cv_not_empty.notify_one();
        }

        Pair pop(){
            std::unique_lock<std::mutex> lock(mtx);
            cv_not_empty.wait(lock, [this]{
                return !queue.empty();
            });
            Pair popped_item = queue.front();
            queue.pop();
            cv_not_full.notify_one();
            return popped_item;
        }
        

};

#endif
