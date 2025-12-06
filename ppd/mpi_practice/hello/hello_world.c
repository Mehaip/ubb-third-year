#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){
    
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); //the amount of processes

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); //the rank of the process

    printf("Hello world from rank %d/%d \n", world_rank, world_size);

    if (world_rank == 0){ ///root
        int number = -1;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if(world_rank == 1){
        int number_received;
        MPI_Recv(&number_received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Hello i am rank %d and i received %d \n", world_rank, number_received);
    }

    MPI_Finalize();
}