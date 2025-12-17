#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class ThreadPool
{
private:
    std::vector<std::thread> threads;        // vector care contine thread-urile
    std::queue<std::function<void()>> tasks; /// queue de functii readFile()
    std::condition_variable cv;              /// variabila conditionala care signal changes in starea task queue-urilor
    std::atomic<bool> stop = false;          /// stop flag
    std::mutex queue_mutex;                  /// mutex pt shared data

public:
    ThreadPool(int num_threads)
    {
        for (int i = 0; i < num_threads; i++)
        {
            threads.emplace_back([this] {
                while (true)
                {
                    function<void()> task;
                    { /// LOCK
                        unique_lock<mutex> lock(queue_mutex);

                        cv.wait(lock, [this]
                                { return !tasks.empty() || stop; }); // worker threads waiting at cv.wait()
                                                                     /// sleeps if: queue empty AND not stopping
                                                                     /// wakes when notified (and rechecks condition)
                        // daca wait e true, se continua rularea
                        if (stop && tasks.empty()) ///daca thread-urile primes notify_all in stop inseamna ca toate au fost procesate, iesim din constructor
                            return;

                        task = tasks.front();
                        tasks.pop();
                    } ///lock gets destroyed here
                    task();
                }
            });
        }
    }

    void enqueue(std::function<void()> task)
    {
        { /// control the lifetime of the lock
            std::lock_guard<std::mutex> lock(queue_mutex);
            tasks.push(task);
        } /// lock gets destroyed here
        cv.notify_one(); // one sleeping thread wakes up
    }

    void shutdown()
    {
        stop = true; /// set stop flag
        cv.notify_all();

        for (auto &t : threads)
        {
            t.join();
        }
    }

    ~ThreadPool()
    {
        if (!stop)
        {
            shutdown();
        }
    }
};

#endif