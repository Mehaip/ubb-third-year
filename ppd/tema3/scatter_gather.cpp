#include <mpi.h>
#include <stdio.h>
#include "file_manager.h"
#include <fstream>

int main(int argc, char *argv[]){

    MPI_Init(&argc, &argv);
    int processId, num_processes;

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    printf("%d - %d\n", processId, num_processes);

    if(processId == 0){
                FileManager fm1("input_files/numar1/1.txt");
        FileManager fm2("input_files/numar2/1.txt");

        std::vector<unsigned char> num1 = fm1.read_number();
        std::vector<unsigned char> num2 = fm2.read_number();


          if (num1.empty() || num2.empty()) {
        printf("ERROR: One or both numbers failed to load!\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
        }
      

        int max_digits = std::max(num1.size(), num2.size());
        num1.resize(max_digits, 0);
        num2.resize(max_digits, 0);
    }

}