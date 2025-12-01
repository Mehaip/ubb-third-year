#ifndef LINKED_LIST_FINE_GRAIN_H
#define LINKED_LIST_FINE_GRAIN_H
#include "parser.h"
#include <mutex>
#include <fstream>

using namespace std;

class Node {
    public:
    Pair data;
    Node* next;
    std::mutex node_mutex;

    Node(Pair value) : data(value), next(nullptr) {};
    
} 

class LinkedListFG {
    private:
        Node* head;
        Node* tail;

    public:
        LinkedListFG(){
            head = new Node(Pair{-1, -1});
            tail = new Node(Pair{-1, -1});
            head->next = tail;
        }

        void addOrUpdate(Pair data){
            Node *prev = head;
            Node *curr = head->next;
            prev->node_mutex.lock();
            curr->node_mutex.lock();
            while(curr != tail){
                if(curr->data.id = data.id){
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
            Node *newNode= Node(data);
            Node *prev = head;
            Node *curr = head->next;

            while(curr != tail){
                curr = curr->next;
            }
        }
}