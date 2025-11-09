#include "file_manager.h"
#include <fstream>

FileManager::FileManager(const std::string &c_fileName)
    : fileName(c_fileName){}

std::vector<unsigned char> FileManager::read_number(){
    std::ifstream fin(this->fileName);
    
    if(!fin.good()){
        printf("ERROR: Cannot open file: %s\n", this->fileName.c_str());
        return std::vector<unsigned char>();
    }
    
    printf("Fisierul este accesat: %s\n", this->fileName.c_str());
    
    int no_of_digits;
    if (!(fin >> no_of_digits)) {
        printf("ERROR: Failed to read number of digits\n");
        return std::vector<unsigned char>();
    }
    printf("DEBUG: no_of_digits = %d\n", no_of_digits);
    
    std::string num;
    if (!(fin >> num)) {
        printf("ERROR: Failed to read number string\n");
        return std::vector<unsigned char>();
    }
    printf("DEBUG: num = %s (length: %zu)\n", num.c_str(), num.length());
    
    if (num.length() != no_of_digits) {
        printf("ERROR: Mismatch! Expected %d digits but got %zu\n", 
               no_of_digits, num.length());
        return std::vector<unsigned char>();
    }
    
    std::vector<unsigned char> digits(no_of_digits);
    for(int i = 0; i < no_of_digits; i++) {
        digits[i] = num[no_of_digits - 1 - i] - '0';
    }
    
    printf("DEBUG: Successfully read %d digits\n", no_of_digits);
    return digits;
}