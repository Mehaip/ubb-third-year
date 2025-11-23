#include "common.h"



void solve_sequential(){
    LinkedList student_list;
    for(int i = 1; i <= 10; i++){
        std::string fileName = "files/proiect" + std::to_string(i) + ".txt";
        std::vector<Pair> pairs = parseFile(fileName);
        for(auto& student_data: pairs){
            student_list.addOrUpdateNode(student_data);
        }
    }
    student_list.saveToFile("files/rezultate.txt");
}

int main(){
    solve_sequential();
}