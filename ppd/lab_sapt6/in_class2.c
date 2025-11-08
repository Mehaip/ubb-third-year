#include <mpi.h>
#include <iostream>

using namespace std;
const int N = 10;

void print(int* a, int n) {
  for (int i = 0; i < n; ++i) {
    std::cout << a[i] << ' ';
  }
  std::cout << std::endl;
}
int main(int argc, char** argv)
{
  int namelen, myid, numprocs;
  MPI_Init(&argc, &argv); // aici initializam mpi -> se creaza un nou communicator, rank process si size
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  int a[N];
  int b[N];
  int c[N];


  MPI_Status status;
  //master are rank 0 by default
  if (myid == 0)
  {
    for (int i = 0; i < N; i++)
    {
      a[i] = rand() % 10;
      b[i] = rand() % 10;
    }

  }
    MPI_Scatter();

    MPI_Finalize();

}