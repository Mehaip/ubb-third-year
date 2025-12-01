#include "linked_list_parallel.h"
#include "linked_list_fine_grain.h"
#include "queue.h"
#include "thread_pool.h"
#include <thread>
#include <vector>
#include <functional>


void worker_function(QueueMutex *data_queue, LinkedList *student_list){
    printf("worker\n");
    while (true) {
        Pair data = data_queue->pop();
        if(data.id == -1) {
            break;
        }
        student_list->addOrUpdate(data);

    }
    printf("gata worker\n");
}

void solve_parallel(int p_r, int p_w){
    std::vector<std::thread> workers;
    LinkedListFG student_list;
    QueueMutex data_queue;

    for(int i = 0; i < p_w; i++){
        printf("worker push back\n");
        workers.push_back(std::thread(worker_function, &data_queue, &student_list));
    }
    
    ///THREAD POOL
    ThreadPool pool(p_r);

    for(int i = 1; i <= 10; i++){
        pool.enqueue([i, &data_queue](){ ///attributes and then the function
            std::string filename = "files/proiect" + std::to_string(i) + ".txt";
            std::vector<Pair> pairs = parseFile(filename);
            for(auto &p: pairs){
                data_queue.push(p);
            }
        });
    }

    pool.shutdown();
    


    for(int i = 0; i < p_w; i++){
        data_queue.push({-1,-1}); //merge
    }

    for(auto& t : workers){
        printf("closed\n");
        t.join();
    }
    printf("push_workers\n");
    student_list.saveToFile("files/rezultate_p.txt");
    printf("save_to_file\n");
}