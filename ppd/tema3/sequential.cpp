#include <stdio.h>
#include "file_manager.h"
#include <fstream>
#include <string>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_number>\n", argv[0]);
        printf("file_number should be 1, 2, or 3\n");
        return 1;
    }

    std::string file_number = argv[1];
    std::string input_file1 = "input_files/numar1/" + file_number + ".txt";
    std::string input_file2 = "input_files/numar2/" + file_number + ".txt";

    FileManager fm1(input_file1.c_str());
    FileManager fm2(input_file2.c_str());

    std::vector<unsigned char> num1 = fm1.read_number();
    std::vector<unsigned char> num2 = fm2.read_number();

    if (num1.empty() || num2.empty()) {
        printf("ERROR: One or both numbers failed to load!\n");
        return 1;
    }

    int max_digits = std::max(num1.size(), num2.size());
    num1.resize(max_digits, 0);
    num2.resize(max_digits, 0);

    std::vector<unsigned char> result;
    unsigned char carry = 0;

    for (int i = 0; i < max_digits; i++) {
        unsigned char sum = num1[i] + num2[i] + carry;
        result.push_back(sum % 10);
        carry = sum / 10;
    }

    if (carry > 0) {
        result.push_back(carry);
    }

    std::string output_file = "output_files/sequential/" + file_number + ".txt";
    std::ofstream fout(output_file.c_str());

    if (!fout.is_open()) {
        printf("ERROR: Could not open output file: %s\n", output_file.c_str());
        return 1;
    }

    for (int i = result.size() - 1; i >= 0; i--) {
        fout << (int)result[i];
    }

    fout.close();


    return 0;
}
