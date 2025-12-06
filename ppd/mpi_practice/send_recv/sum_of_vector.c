#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    MPI_Init(NULL, NULL);
    const int n = 16;
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int *a = (int *)malloc(n * sizeof(int));
    int *b = (int *)malloc(n * sizeof(int));
    int *c = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        a[i] = b[i] = i;
    }
    if (world_rank == 0)
    {

        for (int i = 0; i < n; i++)
        {
            printf("%d ", a[i]);
        }

        printf("\n");

        for (int i = 0; i < n; i++)
        {
            printf("%d ", b[i]);
        }
        printf("\n");

        

        int process_size = n / world_size; /// dimensiunea unui proces
        int remainder = n % world_size;    /// restul in cazul in care exista
        int start = process_size;
        int end;

        for (int process_rank = 1; process_rank < world_size; process_rank++)
        {
            if (remainder)
            {
                end = start + process_size + 1;
                remainder--;
            }
            else
                end = start + process_size;

            MPI_Send(&start, 1, MPI_INT, process_rank, 1, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, process_rank, 2, MPI_COMM_WORLD);
            MPI_Send(a + start, end - start, MPI_INT, process_rank, 3, MPI_COMM_WORLD);
            MPI_Send(b + start, end - start, MPI_INT, process_rank, 4, MPI_COMM_WORLD);
            start = end;
        }

        // Process 0 computes its own portion
        int my_end = process_size;
        if (n % world_size > 0)
            my_end++;
        for (int i = 0; i < my_end; i++)
        {
            c[i] = a[i] + b[i];
        }

        for (int process_rank = 1; process_rank < world_size; process_rank++)
        {
            MPI_Recv(&start, 1, MPI_INT, process_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&end, 1, MPI_INT, process_rank, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(c + start, end - start, MPI_INT, process_rank, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        for (int i = 0; i < n; i++)
            printf("%d ", c[i]);

        free(a);
        free(b);
        free(c);
    }
    else
    {
        int start, end;
        MPI_Recv(&start, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&end, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(a + start, end - start, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b + start, end - start, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("PROCESS %d, START %d, END %d\n", world_rank, start, end);

        for (int i = start; i < end; i++)
        {
            c[i] = a[i] + b[i];
        }

        MPI_Send(&start, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&end, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(c + start, end - start , MPI_INT, 0, 3, MPI_COMM_WORLD);

        free(a);
        free(b);
        free(c);
    }

    MPI_Finalize();
}