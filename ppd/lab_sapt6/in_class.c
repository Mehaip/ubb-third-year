#include <mpi.h>
#include <stdio.h>

int main(int argc, char*argv[]){
    int myId, numProcesses;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);

    printf("%d - %d\n", myId, numProcesses);

    const int n = 16;


    int *a = (int*) malloc(n * sizeof(int));
    int *b = (int*) malloc(n * sizeof(int));
    int *c = (int*) malloc(n * sizeof(int));

    if (myId == 0){
        int cat = n / (numProcesses - 1);
        int rest = n % (numProcesses - 1);
        for (int i = 0; i < n; i++){
            a[i] = i;
            b[i] = i;
        }

        int start = 0;
        int end = cat;

        for(int i = 1; i < numProcesses; i++){
            if (i - 1 < rest){end++;}
            MPI_Send(&start, 1, MPI_INT,i, 0, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT,i, 0, MPI_COMM_WORLD);
            MPI_Send(a + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(b + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
            start = end;
            end = end + cat;
        
    }


        for(int i = 0; i < n; i++){
            printf("%d ", c[i]);
        }

        for(int i = 1; i < numProcesses; i++){
            MPI_Recv(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(c + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        }

        
    }
    else {
        int start, end;
        MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(a + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank: , %d start: %d, end: %d", myId, start, end);

        for(int i = start; i < end; i++){
            c[i] = a[i] + b[i];
        }

        MPI_SEND(&start, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_SEND(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_SEND(c + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}