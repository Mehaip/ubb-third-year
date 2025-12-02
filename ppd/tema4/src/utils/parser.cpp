#include <fstream>
#include "parser.h"

std::vector<Pair> parseFile(const std::string &filename)
{
    std::ifstream fin(filename);

    int id, grade;
    char comma;
    std::vector<Pair> student_data;
    while (fin >> id >> comma >> grade)
    {
        Pair student_info;
        student_info.id = id;
        student_info.grade = grade;
        student_data.push_back(student_info);
    }

    return student_data;
}