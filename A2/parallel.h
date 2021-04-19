#ifndef PARALLEL
#define PARALLEL

#include <omp.h> 
#include <stdio.h>
#include <stdlib.h>

int nThreads = -1;

void crout_par1(int n, double (*A)[n], double (*L)[n], double (*U)[n]){
    // TODO: yahan pe nowait aa skta h?
    #pragma omp parallel for num_threads(nThreads)
    for(int i = 0; i < n; i++){
        U[i][i] = 1;
    }
    #pragma omp parallel num_threads(nThreads) shared(A, L, U)
    {
        for(int j = 0; j < n; j++){
            #pragma omp for schedule(static)
            for(int i = j; i < n; i++){
                double sum = 0;
                for(int k = 0; k < j; k++){
                    sum = sum + L[i][k] * U[k][j];
                }
                L[i][j] = A[i][j] - sum;
            }
            #pragma omp for schedule(static)
            // TODO: yahan pe i = j+1 ho skta h? bcoz u[j][j] = 1 ofc
            for(int i = j; i < n; i++){
                double sum = 0;
                for(int k = 0; k < j; k++){
                    sum = sum + L[j][k] * U[k][i];
                }
                if (L[j][j] == 0){
                    exit(0);
                }
                U[j][i] = (A[j][i] - sum) / L[j][j];
            }
        }
    }
}

void crout_par2(int n, double (*A)[n], double (*L)[n], double (*U)[n]){
    omp_set_num_threads(nThreads);
    int step = n/nThreads;
    // #pragma omp sections
    // {
    //     // int nt = omp_get_thread_num();
    //     // #pragma omp section
    //     // for(int i = n*nt/nThreads; i < n*(nt+1)/nThreads; i++){
    //     //     U[i][i] = 1;
    //     // }
    //     for(int i=0; i<n; i+=step){
    //         #pragma omp section
    //         for(int j=i; j<i+step; j++)
    //             U[j][j] = 1;
    //     }
    // }
    
    #pragma omp parallel num_threads(nThreads) shared(A, L, U)
    {
        for(int j = 0; j < n; j++){
            #pragma omp for schedule(static)
            for(int i = j; i < n; i++){
                double sum = 0;
                for(int k = 0; k < j; k++){
                    sum = sum + L[i][k] * U[k][j];
                }
                L[i][j] = A[i][j] - sum;
            }
            #pragma omp for schedule(static)
            for(int i = j; i < n; i++){
                double sum = 0;
                for(int k = 0; k < j; k++){
                    sum = sum + L[j][k] * U[k][i];
                }
                if (L[j][j] == 0){
                    exit(0);
                }
                U[j][i] = (A[j][i] - sum) / L[j][j];
            }
        }
    }
}

void crout_par3(int n, double (*A)[n], double (*L)[n], double (*U)[n]){
    int i, j, k;
    double sum = 0;
    for(i = 0; i < n; i++){
        U[i][i] = 1;
    }
    for(j = 0; j < n; j++){
        for(i = j; i < n; i++){
            sum = 0;
            for(k = 0; k < j; k++){
                sum = sum + L[i][k] * U[k][j];
            }
            L[i][j] = A[i][j] - sum;
        }
        for(i = j; i < n; i++){
            sum = 0;
            for(k = 0; k < j; k++){
                sum = sum + L[j][k] * U[k][i];
            }
            if (L[j][j] == 0){
                exit(0);
            }
            U[j][i] = (A[j][i] - sum) / L[j][j];
        }
    }
}

#endif
