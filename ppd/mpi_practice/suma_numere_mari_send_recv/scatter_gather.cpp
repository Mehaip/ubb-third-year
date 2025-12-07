#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "file_manager.h"
#include <math.h>
using namespace std;

void print_vector(vector<unsigned char> v)
{
    for (int i = 0; i < v.size(); i++)
        cout << (int)v[i] << " ";
    cout << "\n";
}
int main()
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    FileManager f1 = FileManager("input/nr1.txt");
    FileManager f2 = FileManager("input/nr2.txt");

    vector<unsigned char> num1 = f1.read_number();
    vector<unsigned char> num2 = f2.read_number();

    int digit_count;
    if (world_rank == 0)
    {

        if (num1.empty() || num2.empty())
        {
            printf("ERROR: One or both numbers failed to load!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }
        print_vector(num1);
        cout << num1.size() << "\n";
        print_vector(num2);
        cout << num2.size() << "\n";
        digit_count = std::max(num1.size(), num2.size());
        digit_count = ceil((double)digit_count / world_size) * world_size;
        num1.resize(digit_count, 0);
        num2.resize(digit_count, 0);
        print_vector(num1);
        cout << num1.size() << "\n";
        print_vector(num2);
        cout << num2.size() << "\n";
    }

    MPI_Bcast(&digit_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int digit_count_per_process = digit_count / world_size;

    vector<unsigned char> num1_chunk(digit_count_per_process);
    vector<unsigned char> num2_chunk(digit_count_per_process);

    MPI_Scatter(num1.data(), digit_count_per_process, MPI_UNSIGNED_CHAR,
                num1_chunk.data(), digit_count_per_process, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatter(num2.data(), digit_count_per_process, MPI_UNSIGNED_CHAR,
                num2_chunk.data(), digit_count_per_process, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    int carry = 0;
    vector<unsigned char> result;

    if (world_rank == 0)
    {
        carry = 0;
        for (int i = 0; i < digit_count_per_process; i++)
        {
            int sum = num1_chunk[i] + num2_chunk[i] + carry;
            result.push_back(sum % 10);
            carry = sum / 10;
        }
        MPI_Send(&carry, 1, MPI_INT, world_rank + 1, 1, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&carry, 1, MPI_INT, world_rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < digit_count_per_process; i++)
        {
            int sum = num1_chunk[i] + num2_chunk[i] + carry;
            result.push_back(sum % 10);
            carry = sum / 10;
        }
        if (world_rank < world_size - 1)
            MPI_Send(&carry, 1, MPI_INT, world_rank + 1, 1, MPI_COMM_WORLD);
    }
    int extra_carry = 0;
    if (world_rank == world_size - 1 && carry)
        extra_carry = 1;

    MPI_Bcast(&extra_carry, 1, MPI_INT, world_size - 1, MPI_COMM_WORLD);

    vector<unsigned char> rezultat_final(digit_count);
    MPI_Gather(result.data(), digit_count_per_process, MPI_UNSIGNED_CHAR, rezultat_final.data(),
               digit_count_per_process, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    if (world_rank == 0)
    {
        if (extra_carry)
            cout << 1;
        int zero_check = 0;
        for (int i = digit_count - 1; i >= 0; i--)
        {
            if ((int)(rezultat_final[i]) != 0)
                zero_check = 1;
            if (zero_check)
                cout << (int)(rezultat_final[i]) << " ";
        }
    }
    MPI_Finalize();
}