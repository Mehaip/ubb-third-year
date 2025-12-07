#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

float compute_avg(float *array, int num_elements)
{
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++)
  {
    sum += array[i];
  }
  return sum / num_elements;
}

int main()
{

  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int *a = NULL;
  if (world_rank == 0)
  {
    a = (int *)malloc(16 * sizeof(int));
    for (int i = 0; i < 16; i++)
      a[i] = i;
  }

  int send_count = 16 / world_size;
  int *recv = (int *)malloc(send_count * sizeof(int));

  MPI_Scatter(a, send_count, MPI_INT, recv, send_count, MPI_INT, 0, MPI_COMM_WORLD);

  float local_avg = 0.0f;
  for (int i = 0; i < send_count; i++)
  {
    local_avg += recv[i];
  }
  local_avg /= send_count;

  float *average_list = NULL;
  if (world_rank == 0)
  {
    average_list = (float *)malloc(world_size * sizeof(float));
  }

  MPI_Gather(&local_avg, 1, MPI_FLOAT, average_list, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

  if (world_rank == 0)
  {
    float final_avg = compute_avg(average_list, world_size);
    printf("Average: %f\n", final_avg);
    free(a);
    free(average_list);
  }

  free(recv);
  MPI_Finalize();
  return 0;
}
