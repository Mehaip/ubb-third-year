
#ifndef COMMON_H
#define COMMON_H
#include "parser.h"
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
    
    public:
        LinkedList(){
            head = nullptr;
        }
        bool id_exists(Pair data){
            Node* temp = head;
            while(temp != nullptr){
                if(temp->data.id == data.id){
                    return true;
                }
                temp = temp->next;
            }
        return false;
        }

        void addNode(Pair data){
            Node* newNode = new Node(data);
            if (head == nullptr){
                head = newNode;
            }
            else{
                Node* temp = head;
                while(temp->next!=nullptr)
                    temp = temp->next;
                temp->next = newNode;
        }
        
    }
        void updateNode(Pair data){
            Node* temp = head;
            while(temp!=nullptr){
                if(temp->data.id == data.id)
                    temp->data.grade += data.grade;
                temp=temp->next;
            }
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

