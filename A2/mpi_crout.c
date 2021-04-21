#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include "seq.h"

int main(int argc, char const *argv[]){
     if(argc != 3){
        printf("Invalid usage. Expected: ./executable fileName n\n");
        exit(1);
    }
    int n = atoi(argv[2]);
    
    double **A = allocate_array(n);
    double **L = allocate_array(n);
    double **U = allocate_array(n);
    
    read_input(argv[1], n, A);
    
    MPI_Init(NULL, NULL);

    int comm_size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Finalize();

    free(A);
    free(L);
    free(U);
}


// mpicc -g -Wall -o temp temp.c
// mpiexec -n 3 ./temp

