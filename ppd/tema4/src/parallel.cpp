#include "linked_list_parallel.h"
#include "queue.h"
#include <thread>
#include <vector>

void reader_function(int reader_id, int start, int end, int p_r, QueueMutex *data_queue){

    for(int i = start; i <= end; i++){
        std::string filename = "files/proiect" + std::to_string(i) + ".txt";
        std::vector<Pair> pairs = parseFile(filename);
        for(auto &p: pairs){
            data_queue->push(p);
        }
    }
}

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
    std::vector<std::thread> readers;
    std::vector<std::thread> workers;
    LinkedList student_list;
    QueueMutex data_queue;
    int start = 1, end, remainder;
    remainder = 10%p_r;
    int files_per_reader = 10/p_r;
    for(int i = 0; i < p_r; i++){
        int extra = (remainder > 0) ? 1 : 0;
        end = start + files_per_reader - 1 + extra;
        printf("%d. %d %d\n", i, start, end);
        readers.push_back(std::thread(reader_function, i, start, end, p_r, &data_queue));
        if (remainder > 0)
            remainder--;
        start = end + 1;
    }
    
    for(int i = 0; i < p_w; i++){
        printf("worker push back\n");
        workers.push_back(std::thread(worker_function, &data_queue, &student_list));
    }

    for(auto& t : readers){
        t.join(); //merge

    }

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