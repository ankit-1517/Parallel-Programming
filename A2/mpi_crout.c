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
            if (L[j][j] == 0){
                printf("Zero entry\n");
                exit(0);
            }
            U[j][i] = (A[j][i] - sum2)/L[j][j];
            if(my_rank > 0){
                MPI_Send(&L[i][j], sizeof(double), MPI_DOUBLE, 0, n*i+j, MPI_COMM_WORLD);
                MPI_Send(&U[j][i], sizeof(double), MPI_DOUBLE, 0, n*i+j-1, MPI_COMM_WORLD);
            }
        }
    }

    if(my_rank == 0){
        for(int j=0; j<n; j++){
            for(int i=j; i<n; i++){
                if(i%comm_size == 0)
                    continue;
                MPI_Recv(&L[i][j], sizeof(double), MPI_DOUBLE, i%comm_size, n*i+j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&U[j][i], sizeof(double), MPI_DOUBLE, i%comm_size, n*i+j-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

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


