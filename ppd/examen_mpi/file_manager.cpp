#include <fstream>
#include "file_manager.h"

using namespace std;

FileManager::FileManager(const string &fileName)
    : fileName(fileName) {}

vector<int> FileManager::read_numbers()
{
    ifstream fin(fileName);
    int a;
    vector<int> numbers;
    while (fin >> a)
    {
        numbers.push_back(a);
    }

    return numbers;
};

int FileManager::read_x()
{
    ifstream fin(fileName);
    int x;
    fin >> x;
    return x;
}