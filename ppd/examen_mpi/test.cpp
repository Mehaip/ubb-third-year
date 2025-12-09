#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "file_manager.h"
#include <fstream>
using namespace std;

void print_vector(vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
    cout << "\n";
}

int f_test(int numar, int x)
{
    int sum_cif = 0;
    while (numar)
    {
        sum_cif += numar % 10;
        numar /= 10;
    }
    if (sum_cif < x)
        return 0;
    else
        return 1;
}
int main()
{
    ofstream fout("output.txt");
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    FileManager f = FileManager("numbers.txt");
    FileManager file_x = FileManager("x.txt");
    vector<int> a;
    int x;
    if (world_rank == 0)
    {
        a = f.read_numbers();
        x = file_x.read_x();
    }
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        vector<int> numere_pozitii_pare;
        vector<int> numere_pozitii_impare;
        for (int i = 0; i < a.size(); i++)
        {
            if (i % 2 == 0)
                numere_pozitii_pare.push_back(a[i]);
            else
                numere_pozitii_impare.push_back(a[i]);
        }
        print_vector(a);
        print_vector(numere_pozitii_impare);
        print_vector(numere_pozitii_pare);
        int worker_processes = world_size - 1;
        int even_worker_processes, odd_worker_processes;
        if (worker_processes % 2 == 1)
        {
            even_worker_processes = worker_processes / 2;
            odd_worker_processes = worker_processes / 2 + 1;
        }
        else
        {
            even_worker_processes = odd_worker_processes = worker_processes / 2;
        }
       
        int numbers_half = numere_pozitii_impare.size();

        int numbers_per_worker_process = numbers_half / odd_worker_processes;

        int remainder = numbers_half % odd_worker_processes;
        int start = 0;

        for (int process_rank = 1; process_rank <= worker_processes; process_rank += 2)
        {
            int chunk_size;
            if (remainder)
            {
                chunk_size = numbers_per_worker_process + 1;
                remainder--;
            }
            else
                chunk_size = numbers_per_worker_process;
            
            MPI_Send(&chunk_size, 1, MPI_INT, process_rank, 0, MPI_COMM_WORLD);
            MPI_Send(numere_pozitii_impare.data() + start, chunk_size, MPI_INT, process_rank, 1, MPI_COMM_WORLD);
            start += chunk_size;
        }
        start = 0;
        remainder = numbers_half % even_worker_processes;
        for (int process_rank = 2; process_rank <= worker_processes; process_rank += 2)
        {
            int chunk_size;
            if (remainder)
            {
                chunk_size = numbers_per_worker_process + 1;
                remainder--;
            }
            else
                chunk_size = numbers_per_worker_process;

            MPI_Send(&chunk_size, 1, MPI_INT, process_rank, 0, MPI_COMM_WORLD);
            MPI_Send(numere_pozitii_pare.data() + start, chunk_size, MPI_INT, process_rank, 1, MPI_COMM_WORLD);
            start += chunk_size;
        }
    }
    else
    {

        int A = 0, B = 0;
        if (world_rank % 2 == 1)
        {

            int chunk_size;

            MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vector<int> chunk(chunk_size);
            MPI_Recv(chunk.data(), chunk_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     
            print_vector(chunk);
            for (int i = 0; i < chunk.size(); i++)
            {
                if (f_test(chunk[i], x))
                { /// 0 = suma mai mica
                    chunk[i] *= 2;
                    A++;
                }
                else
                {
                    chunk[i] /= 2;
                    B++;
                }
            }
            for (int i = 0; i < chunk.size(); i++)
                cout << chunk[i] << " ";
                cout<<endl;
        }
        else if (world_rank % 2 == 0 && world_rank != 0)
        {

            int chunk_size;

            MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vector<int> chunk(chunk_size);
            MPI_Recv(chunk.data(), chunk_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
            print_vector(chunk);
            for (int i = 0; i < chunk.size(); i++)
            {
                if (f_test(chunk[i], x))
                { /// 0 = suma mai mica
                    chunk[i] *= 2;
                    A++;
                }
                else
                {
                    chunk[i] /= 2;
                    B++;
                }
            }
         
            for (int i = 0; i < chunk.size(); i++)
                cout << chunk[i] << " ";
            cout << endl;
        }

        MPI_Send(&A, 1, MPI_INT, 1, 3, MPI_COMM_WORLD);
        MPI_Send(&B, 1, MPI_INT, 1, 4, MPI_COMM_WORLD);
    }
    if (world_rank == 1)
    {

        int A_sum = 0, B_sum = 0;
        int A_rec, B_rec;
        for (int i = 1; i < world_size; i++)
        {
            MPI_Recv(&A_rec, 1, MPI_INT, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&B_rec, 1, MPI_INT, i, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
     
            A_sum += A_rec;
            B_sum += B_rec;
        }
        cout << A_sum << " " << B_sum;
    }
    MPI_Finalize();
}