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

    int i, j, k;
    double sum1, sum2;
    for(i = 0; i < n; i++){
        U[i][i] = 1;
    }
    for(j = 0; j < n; j++){
        for(i = j + (comm_size-j%comm_size+my_rank)%comm_size; i < n; i += comm_size){
            sum1 = 0, sum2 = 0;
            for(k = 0; k < j; k++){
                sum1 += L[i][k] * U[k][j];
                sum2 += L[j][k] * U[k][i];
            }
            L[i][j] = A[i][j] - sum1;
            U[j][i] = A[j][i] - sum2;
            // MPI_Send(&L[i][j], 1, MPI_DOUBLE, master, n*i + j, MPI_COMM_WORLD);
            // MPI_Send(&U[j][i], 1, MPI_DOUBLE, master, n*i + j + 1, MPI_COMM_WORLD);
        }
        // MPI_Bcast(&U[j][j], n-j, MPI_DOUBLE, master, MPI_COMM_WORLD);
        for(i=j; i<n; i++){
            MPI_Bcast(&L[i][j], 1, MPI_DOUBLE, i%comm_size, MPI_COMM_WORLD);
            MPI_Bcast(&U[j][i], 1, MPI_DOUBLE, i%comm_size, MPI_COMM_WORLD);
            U[j][i] /= L[j][j];
        }
    }

    if(my_rank == 0){
        char out_file[20];
        snprintf(out_file, 20, "output_L_4_%d.txt", comm_size);
        write_output(out_file, n, L);
        snprintf(out_file, 20, "output_U_4_%d.txt", comm_size);
        write_output(out_file, n, U);
    }

    MPI_Finalize();
    
    free(A);
    free(L);
    free(U);
}


