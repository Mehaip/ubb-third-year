#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){
    
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); //the amount of processes

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); //the rank of the process

    printf("Hello world from rank %d/%d \n", world_rank, world_size);

    MPI_Finalize();
}