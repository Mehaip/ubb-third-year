mpic++ sum_of_vector.c -o exec
mpirun --oversubscribe -np 5 ./exec
