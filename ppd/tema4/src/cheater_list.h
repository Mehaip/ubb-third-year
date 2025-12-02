#ifndef CHEATER_LIST_H
#define CHEATER_LIST_H

#include <mutex>
#include <fstream>

using namespace std;

class NodeCheater
{
public:
    NodeCheater *next;
    int id_student;
    NodeCheater(int id) : id_student(id), next(nullptr) {}
};

class CheaterList
{
private:
    
    mutex mtx;

public:
NodeCheater *head;
    CheaterList() : head(nullptr) {}

    void addCheater(int id_student)
    {
        /// check if student is already in list
        mtx.lock();
        NodeCheater *current = head;

        while (current != nullptr)
        {
            if (current->id_student == id_student)
            {
                mtx.unlock();
                return;
            }
            current = current->next;
        }

        NodeCheater *newNode = new NodeCheater(id_student);
        if (head == nullptr)
        {
            head = newNode;
        }
        else
        {
            NodeCheater *current = head;
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newNode;
        }

        mtx.unlock();
    }

    bool containsCheater(int id_student){
        mtx.lock();
        NodeCheater *current = head;

        while (current != nullptr)
        {
            if (current->id_student == id_student)
            {
                mtx.unlock();
                return true;
            }
            current = current->next;
        }
        mtx.unlock();
        return false;
    }

    void saveCheatersToFile(const std::string &filename)
    {
        NodeCheater *temp = head;
        std::ofstream fout(filename);
        while (temp != nullptr)
        {
            fout << temp->id_student << "\n";
            temp = temp->next;
        }
    }
};

#endif
