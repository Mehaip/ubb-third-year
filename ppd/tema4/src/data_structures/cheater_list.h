#ifndef CHEATER_LIST_H
#define CHEATER_LIST_H

#include <mutex>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class CheaterList
{
private:
    std::vector<int> cheater_ids;  // Vector for random access!
    mutex mtx;

public:
    CheaterList() {}

    void addCheater(int id_student)
    {
        std::lock_guard<std::mutex> lock(mtx);

        // Check if already in list
        if (std::find(cheater_ids.begin(), cheater_ids.end(), id_student) != cheater_ids.end()) {
            return;  // Already exists
        }

        cheater_ids.push_back(id_student);
    }

    bool containsCheater(int id_student)
    {
        std::lock_guard<std::mutex> lock(mtx);
        return std::find(cheater_ids.begin(), cheater_ids.end(), id_student) != cheater_ids.end();
    }

    // Get cheater at index (for parallel access)
    int getCheater(size_t index)
    {
        // No lock needed - only called after all additions are done
        if (index < cheater_ids.size()) {
            return cheater_ids[index];
        }
        return -1;
    }

    // Get total number of cheaters
    size_t size() const
    {
        return cheater_ids.size();
    }

    void saveCheatersToFile(const std::string &filename)
    {
        std::ofstream fout(filename);
        for (int id : cheater_ids) {
            fout << id << "\n";
        }
    }
};

#endif
