#include "linked_list_fine_grain.h"
#include "linked_list_parallel.h"
#include "sorted_linked_list_fine_grain.h"
#include "queue.h"
#include "thread_pool.h"
#include <thread>
#include <vector>
#include <functional>
#include "cheater_list.h"
#include <atomic>
#include <barrier>
#include <sqlite3.h>

template <typename BarrierType>
void worker_function(BarrierType *worker_barrier, SortedLinkedListFG *sorted_student_list,
                     QueueMutex *data_queue, LinkedListFG *student_list, CheaterList *cheater_list)
{

    while (true)
    {
        Pair data = data_queue->pop();
        if (data.id == -1)
        {
            break;
        }
        if (data.grade == -1)
        {
            cheater_list->addCheater(data.id);
        }
        else
        {
            student_list->addOrUpdate(data);
        }
    }

    worker_barrier->arrive_and_wait(); // asteptam sa termine worker procesurile

    while (true)
    {

        Pair nodeData = student_list->removeFirst();
        if (nodeData.id == -1)
            break;
        sorted_student_list->add(nodeData);
    }
}

void solve_parallel(int p_r, int p_w)
{
    std::vector<std::thread> workers;
    LinkedListFG student_list;
    QueueMutex data_queue;
    CheaterList cheaters;
    SortedLinkedListFG sorted_student_list;
    std::barrier worker_barrier(p_w, /// callback function care sterge toti cheaterii
                                [&]()
                                {
                                    NodeCheater *node = cheaters.head;
                                    while (node != nullptr)
                                    {
                                        student_list.removeCheater(node->id_student);
                                        node = node->next;
                                    }
                                });
    for (int i = 0; i < p_w; i++)
    {
        printf("worker push back\n");
        workers.push_back(std::thread([&]()
                                      { worker_function(&worker_barrier, &sorted_student_list, &data_queue, &student_list, &cheaters); }));
    }

    /// THREAD POOL
    ThreadPool pool(p_r);

    for (int i = 1; i <= 10; i++)
    {
        pool.enqueue([i, &data_queue]() { /// attributes and then the function
            std::string filename = "files/input/proiect" + std::to_string(i) + ".txt";
            std::vector<Pair> pairs = parseFile(filename);
            for (auto &p : pairs)
            {
                data_queue.push(p);
            }
        });
    }

    pool.shutdown();

    for (int i = 0; i < p_w; i++)
    {
        data_queue.push({-1, -1}); // merge
    }

    for (auto &t : workers)
    {
        printf("closed\n");
        t.join();
    }
    printf("push_workers\n");

    sorted_student_list.saveToFile("files/output/rezultate_p.txt");
    cheaters.saveCheatersToFile("files/output/cheaters_p.txt");
    printf("save_to_file\n");
}