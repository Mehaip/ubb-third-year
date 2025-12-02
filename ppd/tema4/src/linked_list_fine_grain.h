#ifndef LINKED_LIST_FINE_GRAIN_H
#define LINKED_LIST_FINE_GRAIN_H
#include "parser.h"
#include <mutex>
#include <fstream>

using namespace std;

class NodeMutex {
    public:
    Pair data;
    NodeMutex* next;
    std::mutex node_mutex;

    NodeMutex(Pair value) : data(value), next(nullptr) {};
    
}; 

class LinkedListFG {
    private:
        NodeMutex* head;
        NodeMutex* tail;

    public:
        LinkedListFG(){
            head = new NodeMutex(Pair{-1, -1});
            tail = new NodeMutex(Pair{-1, -1});
            head->next = tail;
        }

        ~LinkedListFG(){
            NodeMutex* curr = head;
            while(curr != nullptr){
                NodeMutex* next = curr->next;
                delete curr;
                curr = next;
            }
        }

        void addOrUpdate(Pair data){
            NodeMutex *prev = head;
            prev->node_mutex.lock();

            NodeMutex *curr = head->next;
            curr->node_mutex.lock();
            while(curr != tail){
                if(curr->data.id == data.id){
                    curr->data.grade += data.grade;
                    prev->node_mutex.unlock();
                    curr->node_mutex.unlock();
                    return;
                }
                prev->node_mutex.unlock();
                prev = curr;
                curr = curr->next;
                curr->node_mutex.lock();
            }
            ///not found in list
            NodeMutex *newNode= new NodeMutex(data);
            newNode->next = curr;
            prev->next = newNode;

            curr->node_mutex.unlock();
            prev->node_mutex.unlock();
        };

        void removeCheater(int id){
            NodeMutex *prev = head;
            prev->node_mutex.lock();
            NodeMutex *curr = head->next;
            curr->node_mutex.lock();
            while(curr != tail){
                if(curr->data.id == id){
                    prev->next = curr->next;
                    prev->node_mutex.unlock();
                    curr->node_mutex.unlock();
                    delete curr;
                    return;
                }
                prev->node_mutex.unlock();
                prev = curr;
                curr = curr->next;
                curr->node_mutex.lock();
            }
            curr->node_mutex.unlock();
            prev->node_mutex.unlock();

        }

        void saveToFile(const std::string& filename){
            NodeMutex* temp = head->next;
            std::ofstream fout(filename);
            while(temp != tail){
                fout << temp->data.id <<"," << temp->data.grade << "\n";
                temp = temp->next;
            }
        }
};
#endif