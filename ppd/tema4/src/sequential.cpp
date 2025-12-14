#include "linked_list_parallel.h"
#include "linked_list_fine_grain.h"
#include "sorted_linked_list_fine_grain.h"
#include "cheater_list.h"

void solve_sequential()
{
    LinkedListFG student_list;
    CheaterList cheaters;
    SortedLinkedListFG sorted_student_list;
    std::string db_path = "students.db";

    // Read data from database and populate student_list
    for (int i = 1; i <= 10; i++)
    {
        std::vector<Pair> pairs = parseDatabaseTable(db_path, i);
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

    // Remove cheaters from student_list
    for (size_t i = 0; i < cheaters.size(); i++) {
        int cheater_id = cheaters.getCheater(i);
        if (cheater_id != -1) {
            student_list.removeCheater(cheater_id);
        }
    }

    // Move all nodes from student_list to sorted_student_list
    while (true)
    {
        Pair nodeData = student_list.removeFirst();
        if (nodeData.id == -1)
            break;
        sorted_student_list.add(nodeData);
    }

    // Save results
    sorted_student_list.saveToFile("files/output/rezultate.txt");
    cheaters.saveCheatersToFile("files/output/cheaters.txt");
}
