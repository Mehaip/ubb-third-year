
#ifndef LINKED_LIST_PARALLEL_H
#define LINKED_LIST_PARALLEL_H
#include "parser.h"
#include <mutex>
#include <fstream>
class Node{
    public:
        Pair data;
        Node* next;
        Node(Pair value) : data(value), next(nullptr) {}
};

class LinkedList{
    private:
        Node* head;
        std::mutex mtx;
    
    public:
        LinkedList(){
            head = nullptr;
        }
        
        void addOrUpdate(Pair data){
            mtx.lock();
            Node* temp = head;
            while(temp != nullptr){ 
                if(temp->data.id == data.id){ ///nodul se afla in lista
                    temp->data.grade += data.grade;
                    mtx.unlock();
                    return;
                }
                temp = temp->next;
            }
            Node* newNode = new Node(data);
            if(head == nullptr){
                head = newNode;
            } else {
            Node* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = newNode;  ///temp->next = temp
        }
            mtx.unlock();

        }

        void saveToFile(const std::string& filename){
            Node* temp = head;
            std::ofstream fout(filename);
            while(temp != nullptr){
                fout << temp->data.id <<"," << temp->data.grade << "\n";
                temp = temp->next;
            }
        }

};

#endif

