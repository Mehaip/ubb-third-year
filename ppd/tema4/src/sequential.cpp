#include "linked_list_parallel.h"
#include "cheater_list.h"
void solve_sequential()
{
    LinkedList student_list;
    CheaterList cheaters;
    for (int i = 1; i <= 10; i++)
    {
        std::string fileName = "files/proiect" + std::to_string(i) + ".txt";
        std::vector<Pair> pairs = parseFile(fileName);
        for (auto &student_data : pairs)
        {
            if (student_data.grade == -1)
            {
                cheaters.addCheater(student_data.id);
            }
            else
            {
                    student_list.addOrUpdate(student_data);
                    
            }
        }
    }

    NodeCheater* node = cheaters.head;
    while(node != nullptr){
        student_list.removeCheater(node->id_student);
        node = node->next;
    }

    student_list.saveToFile("files/rezultate.txt");
    cheaters.saveCheatersToFile("files/cheaters.txt");
}
