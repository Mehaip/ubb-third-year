#include <mpi.h>
#include <stdio.h>
#include "file_manager.h"
#include <fstream>
int main(int argc, char*argv[]){

    MPI_Init(&argc, &argv);
    int processId, num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    printf("%d - %d\n", processId, num_processes);

    int num_workers = num_processes - 1;

    if (processId == 0){
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
    
        int total_digits = max_digits;
        int base_chunk_size = total_digits / num_workers;
        int remainder = total_digits % num_workers;
        int start_index = 0;
        for(int worker = 1; worker < num_processes; worker++){
            int chunk_size = base_chunk_size + remainder;
            printf("CHUNKSIZE: %d\n", chunk_size);
            MPI_Send(&chunk_size, 1, MPI_INT, worker,0, MPI_COMM_WORLD);

            MPI_Send(&num1[start_index],chunk_size,MPI_UNSIGNED_CHAR,worker,1,MPI_COMM_WORLD);
            MPI_Send(&num2[start_index], chunk_size, MPI_UNSIGNED_CHAR, worker, 2, MPI_COMM_WORLD);
            start_index += chunk_size;
            if(num_workers - worker > 0)
            remainder = (total_digits-start_index) % (num_workers-worker);
            
        }
        for(int i = 1; i < num_processes; i++){
            int end_signal = 0;
            MPI_Send(&end_signal, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        std::ofstream fout("numar3.txt");
        std::vector <unsigned char> result;

        for (int i = num_processes-1; i > 0; i--){ ///WOW!! INCEPEM DE LA ULTIMUL THREAD PT CA EL ARE CELE MAI IMPORTANTE NUMERE
            int result_size;
            MPI_Recv(&result_size, 1, MPI_INT, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (result_size > 0){
                std::vector <unsigned char> chunk(result_size);
                MPI_Recv(chunk.data(), result_size, MPI_UNSIGNED_CHAR, i, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for (int i = result_size - 1; i >= 0; i--){
                    fout << (int)chunk[i];
                }
                
            }
        }
    
        fout.close();
        
    }
    else {
        printf("DEBUG: Thread %d\n", processId);
        unsigned char carry = 0;
        std::vector<unsigned char> my_result;

        while (true){
            int chunk_size;
            MPI_Recv(&chunk_size, 1,MPI_INT,0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if(chunk_size == 0) break;

            std::vector<unsigned char> chunk1 (chunk_size);
            std::vector<unsigned char> chunk2 (chunk_size);

            MPI_Recv(chunk1.data(), chunk_size, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(chunk2.data(), chunk_size, MPI_UNSIGNED_CHAR, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            ///calculam cu carry = 0 initial
            for(int i = 0; i < chunk_size; i++){
                unsigned char sum = chunk1[i] + chunk2[i] + carry;
                my_result.push_back(sum % 10);
                carry = sum / 10;
            }
        }
         if (processId == num_processes - 1 && carry > 0){
                    my_result.push_back(carry);
                    printf("carry!");
            }

            if (processId > 1){
                unsigned char incoming_carry;
                MPI_Recv(&incoming_carry, 1, MPI_UNSIGNED_CHAR, processId - 1, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                carry = incoming_carry;
                for (size_t i = 0; i < my_result.size() && carry > 0; i++){
                    unsigned char sum = my_result[i] + carry;
                    my_result[i] = sum % 10;
                    carry = sum / 10;
                }
            }



            if(processId < num_processes - 1) {///trimitem carryu la urmatorul id, decat daca procesul nu e ultimul
                MPI_Send(&carry, 1, MPI_UNSIGNED_CHAR, processId + 1, 5, MPI_COMM_WORLD);
                }
            


            int result_size = my_result.size();
            printf("Thread %d: %d\n", processId, result_size);
            MPI_Send(&result_size, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
            if (result_size > 0){
                MPI_Send(my_result.data(), result_size, MPI_UNSIGNED_CHAR, 0, 4, MPI_COMM_WORLD);
            }


}
   MPI_Finalize();
}