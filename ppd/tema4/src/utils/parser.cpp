#include <fstream>
#include <sqlite3.h>
#include <iostream>
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

std::vector<Pair> parseDatabaseTable(const std::string &db_path, int project_number)
{
    std::vector<Pair> student_data;
    sqlite3 *db;
    sqlite3_stmt *stmt;

    // Open database connection
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return student_data;
    }

    // Prepare SQL query
    std::string table_name = "proiect" + std::to_string(project_number);
    std::string sql = "SELECT id, grade FROM " + table_name + ";";

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return student_data;
    }

    // Execute query and fetch results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        Pair student_info;
        student_info.id = sqlite3_column_int(stmt, 0);
        student_info.grade = sqlite3_column_int(stmt, 1);
        student_data.push_back(student_info);
    }

    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error during query execution: " << sqlite3_errmsg(db) << std::endl;
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return student_data;
}