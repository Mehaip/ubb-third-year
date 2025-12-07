#include <mpi.h>
#include <stdio.h>
#include "file_manager.h"
#include <fstream>
#include <string>
#include "math.h"

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
    int processId, num_processes;

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    // Validate command-line arguments
    if (argc != 2)
    {
        if (processId == 0)
        {
            printf("Usage: %s <file_number>\n", argv[0]);
            printf("file_number should be 1, 2, or 3\n");
        }
        MPI_Finalize();
        return 1;
    }

    std::string file_number = argv[1];
    std::string input_file1 = "input_files/numar1/" + file_number + ".txt";
    std::string input_file2 = "input_files/numar2/" + file_number + ".txt";

    // printf("%d - %d\n", processId, num_processes);
    FileManager fm1(input_file1.c_str());
    FileManager fm2(input_file2.c_str());
    std::vector<unsigned char> num1 = fm1.read_number();
    std::vector<unsigned char> num2 = fm2.read_number();
    int digit_count;
    if (processId == 0)
    {

        if (num1.empty() || num2.empty())
        {
            printf("ERROR: One or both numbers failed to load!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }

        digit_count = std::max(num1.size(), num2.size());
        digit_count = ceil(digit_count / num_processes) * num_processes;
        num1.resize(digit_count, 0);
        num2.resize(digit_count, 0);
    }
    MPI_Bcast(&digit_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int digit_count_per_process = digit_count / num_processes;
    std::vector<unsigned char> chunk_nr1(digit_count_per_process);
    std::vector<unsigned char> chunk_nr2(digit_count_per_process);

    MPI_Scatter(num1.data(), digit_count_per_process, MPI_UNSIGNED_CHAR, chunk_nr1.data(), digit_count_per_process, MPI_UNSIGNED_CHAR,
                0, MPI_COMM_WORLD);

    MPI_Scatter(num2.data(), digit_count_per_process, MPI_UNSIGNED_CHAR, chunk_nr2.data(), digit_count_per_process, MPI_UNSIGNED_CHAR,
                0, MPI_COMM_WORLD);

    std::vector<unsigned char> my_result;
    unsigned char carry = 0;

    /// calculam locally
    for (int i = 0; i < digit_count_per_process; i++)
    {
        unsigned char sum = chunk_nr1[i] + chunk_nr2[i] + carry;
        my_result.push_back(sum % 10);
        carry = sum / 10;
    }
    unsigned char extra_digit = 0;
    bool carry_after_local = false;
    if (processId == num_processes - 1 && carry > 0)
    {
        carry_after_local = true;
    }

    if (processId > 0)
    {
        unsigned char incoming_carry;
        MPI_Recv(&incoming_carry, 1, MPI_UNSIGNED_CHAR, processId - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < digit_count_per_process; i++)
        {
            unsigned char sum = my_result[i] + incoming_carry;
            my_result[i] = sum % 10;
            incoming_carry = sum / 10;
        }
        carry = incoming_carry;
    }
    if (processId == num_processes - 1)
    { /// EROARE
        if (carry > 0 || carry_after_local)
        {
            extra_digit = 1;
        }
        MPI_Send(&extra_digit, 1, MPI_UNSIGNED_CHAR, 0, 2, MPI_COMM_WORLD);
    }

    if (processId < num_processes - 1)
    {
        MPI_Send(&carry, 1, MPI_UNSIGNED_CHAR, processId + 1, 1, MPI_COMM_WORLD);
    }
    std::vector<unsigned char> all_results;
    if (processId == 0)
    {
        all_results.resize(digit_count);
        MPI_Recv(&extra_digit, 1, MPI_UNSIGNED_CHAR, num_processes - 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Gather(my_result.data(), digit_count_per_process, MPI_UNSIGNED_CHAR,
               all_results.data(), digit_count_per_process, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    if (processId == 0)
    {
        std::string output_file = "output_files/scatter_gather/" + file_number + ".txt";
        std::ofstream fout(output_file.c_str());
        /// acum all_results contine rezultatul de la thread 0 la thread noThreads - 1
        if (extra_digit)
            fout << "1";
        for (int i = digit_count - 1; i >= 0; i--)
        {
            fout << (int)(all_results[i]);
        }
    }

    MPI_Finalize();
}