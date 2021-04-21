#include "seq.h"
#include "parallel.h"

int main(int argc, char const *argv[]){
    if(argc != 5){
        printf("Invalid usage. Expected: ./executable fileName n strategy nThreads\n");
        exit(1);
    }
    nThreads = atoi(argv[4]);
    int n = atoi(argv[2]);
    int strategy = atoi(argv[3]);
    
    double **A = allocate_array(n);
    double **L = allocate_array(n);
    double **U = allocate_array(n);

    read_input(argv[1], n, A);
    
    switch(strategy){
        case 0:
            crout(n, A, L, U);
            break;
        case 1:
            crout_par1(n, A, L, U);
            break;
        case 2:
            crout_par2(n, A, L, U);
            break;
        case 3:
            crout_par3(n, A, L, U);
            break;
        default:
            printf("Invalid strategy. Exiting...\n");
            exit(0);
    }

    char out_file[20];
    snprintf(out_file, 20, "output_L_%d_%d.txt", strategy, nThreads);
    write_output(out_file, n, L);
    snprintf(out_file, 20, "output_U_%d_%d.txt", strategy, nThreads);
    write_output(out_file, n, U);

    free(A);
    free(L);
    free(U);
    return 0;
}

