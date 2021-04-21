#ifndef PARALLEL
#define PARALLEL

#include <omp.h> 
#include <stdio.h>
#include <stdlib.h>

int nThreads = -1;

void crout_par1(int n, double **A, double **L, double **U){
    #pragma omp parallel for num_threads(nThreads)
    for(int i = 0; i < n; i++){
        U[i][i] = 1;
    }
    for(int j = 0; j < n; j++){
	    #pragma omp parallel num_threads(nThreads) shared(A, L, U)
	    {
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
                    printf("What the fuck\n");
                    exit(0);
                }
                U[j][i] = (A[j][i] - sum) / L[j][j];
            }
        }
    }
}
void task_2(int n, double **A, double **L, double **U, int j, int i){
    double sum = 0;
    for(int k = 0; k < j; k++){
        sum = sum + L[i][k] * U[k][j];
    }
    L[i][j] = A[i][j] - sum;
    sum = 0;
    for(int k = 0; k < j; k++){
        sum = sum + L[j][k] * U[k][i];
    }
    if (L[j][j] == 0){
        exit(0);
    }
    U[j][i] = (A[j][i] - sum) / L[j][j];
}
void crout_par2(int n, double **A, double **L, double **U){
    omp_set_num_threads(nThreads);
    for(int i = 0; i < n; i++){
        U[i][i] = 1;
    }    
 	for(int j = 0; j < n; j++){
        double sum = 0;
        for(int k = 0; k < j; k++){
            sum = sum + L[j][k] * U[k][j];
        }
        L[j][j] = A[j][j] - sum;
        if (L[j][j] == 0){
            exit(0);
        }
        U[j][j] = (A[j][j] - sum) / L[j][j];
        int count = (n-1-j)/8;
        #pragma omp parallel sections
        {
        	#pragma omp section
        	{
		       	for(int i = j+1; i < j+1+count ; i++){
		       		task_2(n, A, L, U, j, i);
		        }
        	}
        	#pragma omp section
        	{
		       	for(int i = j+1+count; i < j+1+2*count ; i++){
		       		task_2(n, A, L, U, j, i);
		        }      		
        	}
        	#pragma omp section
        	{
		       	for(int i = j+1+2*count; i < j+1+3*count ; i++){
		       		task_2(n, A, L, U, j, i);
		        }        		
        	}
        	#pragma omp section
        	{
		       	for(int i = j+1+3*count; i < j+1+4*count ; i++){
		       		task_2(n, A, L, U, j, i);
		        }        		
        	}
        	#pragma omp section
        	{
		       	for(int i = j+1+4*count; i < j+1+5*count ; i++){
		       		task_2(n, A, L, U, j, i);
		        }          		
        	}
        	#pragma omp section
        	{
		       	for(int i = j+1+5*count; i < j+1+6*count ; i++){
		       		task_2(n, A, L, U, j, i);
		        }        		
        	}
        	#pragma omp section
        	{
		       	for(int i = j+1+6*count; i < j+1+7*count ; i++){
		       		task_2(n, A, L, U, j, i);
		        }           		
        	}
        	#pragma omp section
        	{
		       	for(int i = j+1+7*count; i < n ; i++){
		       		task_2(n, A, L, U, j, i);
		        }           		        		
        	}        	
        }  		

    }
}

void Task1(int n, double **A,double **L, double **U, int j){
	#pragma omp parallel for schedule(static) num_threads(nThreads/2)
    for(int i = j+1; i < n; i++){
        double sum = 0;
        for(int k = 0; k < j; k++){
            sum = sum + L[i][k] * U[k][j];
        }
        L[i][j] = A[i][j] - sum;
    }
}
void Task2(int n, double **A,double **L, double **U, int j){
	#pragma omp parallel for schedule(static) num_threads(nThreads/2)
    for(int i = j+1; i < n; i++){
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
void doTask(int n, double **A,double **L, double **U, int j, int i){
	#pragma omp parallel sections 
	{
		#pragma omp section
		{
	        double sum = 0;
	        for(int k = 0; k < j; k++){
	            sum = sum + L[i][k] * U[k][j];
	        }
	        L[i][j] = A[i][j] - sum;
		}
		#pragma omp section
		{
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


void crout_par3(int n, double **A, double **L, double **U){
    #pragma omp parallel for num_threads(nThreads)
    for(int i = 0; i < n; i++){
        U[i][i] = 1;
    }

  //   for(int j = 0; j < n; j++){
  //   	// for i == j
  //       double sum = 0;
  //       double localsum = 0;
  //       omp_set_nested(0);
  //      	#pragma omp parallel num_threads(nThreads) shared(sum) private(localsum)
  //   	{
	 //        localsum = 0;

	 //        #pragma omp for nowait
	 //        for(int k = 0; k < j; k++){
	 //            localsum = localsum + L[j][k] * U[k][j];
	 //        }

	 //        #pragma omp critical
	 //        {
	 //            sum += localsum;
	 //        }

  //   	} 
  //       L[j][j] = A[j][j] - sum;
  //       if (L[j][j] == 0){
  //           exit(0);
  //       }
  //       U[j][j] = (A[j][j] - sum) / L[j][j];
  //       omp_set_nested(1);
		// #pragma omp parallel for schedule(dynamic) num_threads(nThreads)
	 //    for(int i = j+1; i < n; i++){
	 //    	doTask(n, A, L, U, j, i);
	 //    } 
  //   }
    omp_set_nested(1);
    for(int j = 0; j < n; j++){
    	// for i == j
        double sum = 0;
        for(int k = 0; k < j; k++){
            sum = sum + L[j][k] * U[k][j];
        }
        L[j][j] = A[j][j] - sum;
        if (L[j][j] == 0){
            exit(0);
        }
        U[j][j] = (A[j][j] - sum) / L[j][j]; 
        #pragma omp parallel sections 
        {
        	#pragma omp section
        	{
        		Task1(n, A, L, U, j);
        	}
        	#pragma omp section
        	{
        		Task2(n, A, L, U, j);
        	}
    	}   	
    }

    
}

#endif
