#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"
#include <iostream>
using namespace std;

void print_vector(vector<unsigned char> v)
{
    for (int i = 0; i < v.size(); i++)
        cout << (int)v[i] << " ";
    cout << "\n";
}

void compute_sum(int *carry, vector<unsigned char> *result, vector<unsigned char> *num1, vector<unsigned char> *num2, int chunk_size)
{

    for (int i = 0; i < chunk_size; i++)
    {
        int a = (*num1)[i] + (*num2)[i] + *carry;
        if (a > 9)
            *carry = 1;
        else
            *carry = 0;
        (*result)[i] = a % 10;
    }

}

int main()
{

    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    FileManager f = FileManager("input/nr1.txt");
    FileManager f2 = FileManager("input/nr2.txt");

    if (world_rank == 0)
    {
        vector<unsigned char> num1 = f.read_number();
        vector<unsigned char> num2 = f2.read_number();

        int number_size = num1.size();
        int chunk_size = number_size / (world_size - 1);
        int start = 0;
        int remainder = number_size % (world_size - 1);
        print_vector(num1);
        print_vector(num2);
        for (int process_rank = 1; process_rank < world_size; process_rank++)
        {
            int chunk_size_send = chunk_size;
            if (remainder)
            {
                chunk_size_send++;
                remainder--;
            }
            MPI_Send(&chunk_size_send, 1, MPI_INT, process_rank, 0, MPI_COMM_WORLD);
            MPI_Send(num1.data() + start, chunk_size_send, MPI_UNSIGNED_CHAR, process_rank, 1, MPI_COMM_WORLD);
            MPI_Send(num2.data() + start, chunk_size_send, MPI_UNSIGNED_CHAR, process_rank, 2, MPI_COMM_WORLD);
            start += chunk_size_send;
        }

        for (int process_rank = world_size - 1; process_rank >= 1; process_rank--)
        {
            int chunk_size;
            MPI_Recv(&chunk_size, 1, MPI_INT, process_rank, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vector<unsigned char> result(chunk_size);
            MPI_Recv(result.data(), chunk_size, MPI_UNSIGNED_CHAR, process_rank, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = chunk_size - 1; i >= 0; i--)
                cout << (int)result[i];
        }
    }
    else
    {

        int chunk_size;
        MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        vector<unsigned char> num1_chunk(chunk_size), num2_chunk(chunk_size);
        MPI_Recv(num1_chunk.data(), chunk_size, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(num2_chunk.data(), chunk_size, MPI_UNSIGNED_CHAR, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        vector<unsigned char> result(chunk_size);

        if (world_rank == 1)
        {
            int carry = 0;
            compute_sum(&carry, &result, &num1_chunk, &num2_chunk, chunk_size);
            if (world_rank != world_size - 1)
                MPI_Send(&carry, 1, MPI_INT, world_rank + 1, 3, MPI_COMM_WORLD);
            if (world_rank == world_size - 1 && carry != 0)
                cout << 1;
            MPI_Send(&chunk_size, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
            MPI_Send(result.data(), chunk_size, MPI_UNSIGNED_CHAR, 0, 6, MPI_COMM_WORLD);
        }
        else
        {
            int carry;
            MPI_Recv(&carry, 1, MPI_INT, world_rank - 1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            compute_sum(&carry, &result, &num1_chunk, &num2_chunk, chunk_size);

            if (world_rank != world_size - 1)
                MPI_Send(&carry, 1, MPI_INT, world_rank + 1, 3, MPI_COMM_WORLD);
            if (world_rank == world_size - 1 && carry != 0)
                cout << 1;
            MPI_Send(&chunk_size, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
            MPI_Send(result.data(), chunk_size, MPI_UNSIGNED_CHAR, 0, 6, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
}