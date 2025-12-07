#include <fstream>
#include "file_manager.h"

using namespace std;

FileManager::FileManager(const string &fileName)
    : fileName(fileName){}

vector<unsigned char> FileManager::read_number(){
    ifstream fin(fileName);
    string input;
    fin >> input;
    vector<unsigned char> number;
    for(int i = input.length() - 1; i >= 0; i--){
        number.push_back(input[i] - '0');
    }

    return number;
}