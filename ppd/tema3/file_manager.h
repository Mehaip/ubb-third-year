#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

class FileManager{
    const std::string fileName;
    
public:
    FileManager(const std::string &c_fileName);
    
    std::vector<unsigned char> read_number();
};

#endif