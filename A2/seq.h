#ifndef SEQ
#define SEQ

#include <stdio.h>
#include <stdlib.h>

void write_output(char const fname[], int n, double **arr){
    FILE *f = fopen(fname, "w");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            fprintf(f, "%0.12f ", arr[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void read_input(char const fname[], int n, double **arr){
    FILE *f = fopen(fname, "r");
    if(!f){
        printf("File %s doesn't exist. Exiting...\n", fname);
        exit(1);
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            fscanf(f, "%lf", &arr[i][j]);
        }
    }
    fclose(f);
}

double** allocate_array(int n){
    double** arr = malloc(n*sizeof(double*));
    for(int i=0; i<n; i++)
        arr[i] = malloc(n*sizeof(double));
    return arr;
}

void crout(int n, double **A, double **L, double **U){
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
