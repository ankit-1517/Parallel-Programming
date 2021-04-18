#ifndef PARALLEL
#define PARALLEL

#include <omp.h> 
#include <stdio.h>
#include <stdlib.h>

int nThreads = -1;

void crout_par1(int n, double (*A)[n], double (*L)[n], double (*U)[n]){
    int i, j, k;
    double sum = 0;
    #pragma omp parallel for num_threads(nThreads)
    for(i = 0; i < n; i++){
        U[i][i] = 1;
    }
    #pragma omp parallel for num_threads(nThreads)
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

void crout_par2(int n, double (*A)[n], double (*L)[n], double (*U)[n]){
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
