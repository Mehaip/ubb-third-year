#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int compute_sum(vector<int> array, int array_size)
{
    int sum = 0;
    for (int i = 0; i < array_size; i++)
        sum += array[i];
    return sum;
}

void print_vector(vector<int> array)
{
    for (auto i : array)
    {
        printf("%d ", i);
        
    }
    printf("\n");
}

int main()
{

    const int n = 16;
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    vector<int> a;

    if (world_rank == 0)
    {
        for (int i = 0; i < n; i++)
        {
            a.push_back(i);
        }
    }

    int data_count = n / world_size;
    vector<int> receive_vector;
    receive_vector.resize(data_count);
    MPI_Scatter(a.data(), data_count, MPI_INT, receive_vector.data(), data_count, MPI_INT, 0, MPI_COMM_WORLD);
    int sum = 0;

    sum = compute_sum(receive_vector, data_count);
    vector<int> sum_array;
    sum_array.resize(world_size);
    printf("Process %d with sum %d\n", world_rank, sum);
    MPI_Gather(&sum, 1, MPI_INT, sum_array.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        print_vector(sum_array);
        sum = compute_sum(sum_array, world_size);
        printf("Process %d with sum %d\n", world_rank, sum);
    }

    MPI_Finalize();
}