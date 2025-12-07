#ifndef FILE_MANAGER
#define FILE_MANAGER

#include <string>
#include <vector>

using namespace std;

class FileManager
{
    const string fileName;

public:
    FileManager(const string &fileName);
    vector<unsigned char> read_number();
};

#endif