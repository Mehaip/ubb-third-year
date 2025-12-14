#ifndef SORTED_LINKED_LIST_FINE_GRAIN_H
#define SORTED_LINKED_LIST_FINE_GRAIN_H

#include "parser.h"
#include <mutex>
#include <fstream>
#include "linked_list_fine_grain.h"

using namespace std;

class SortedLinkedListFG
{
private:
    NodeMutex *head;
    NodeMutex *tail;

public:
    SortedLinkedListFG()
    {
        head = new NodeMutex(Pair{-1, 9999});
        tail = new NodeMutex(Pair{-1, -9999});
        head->next = tail;
    }
    ~SortedLinkedListFG()
    {
        NodeMutex *curr = head;
        while (curr != nullptr)
        {
            NodeMutex *next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void add(Pair data)
    {
        NodeMutex *newNode = new NodeMutex(data);

        NodeMutex *prev = head;
        prev->node_mutex.lock();

        NodeMutex *curr = prev->next;
        curr->node_mutex.lock();

        while (curr != tail)
        {
            if (newNode->data.grade >= curr->data.grade)
            {
                newNode->next = curr;
                prev->next = newNode;

                curr->node_mutex.unlock();
                prev->node_mutex.unlock();
                return;
            }

            prev->node_mutex.unlock();
            prev = curr;
            curr = curr->next;
            curr->node_mutex.lock();
        }
        newNode->next = curr;
        prev->next = newNode;

        curr->node_mutex.unlock();
        prev->node_mutex.unlock();
    }

    void saveToFile(const std::string &filename)
    {
        NodeMutex *temp = head->next;
        std::ofstream fout(filename);
        while (temp != tail)
        {
            fout << temp->data.id << "," << temp->data.grade << "\n";
            temp = temp->next;
        }
    }
};

#endif