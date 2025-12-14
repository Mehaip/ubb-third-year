#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

struct Pair
{
    int id;
    int grade;
};

std::vector<Pair> parseFile(const std::string &filename);
std::vector<Pair> parseDatabaseTable(const std::string &db_path, int project_number);

#endif