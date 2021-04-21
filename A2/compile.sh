# !/bin/bash
gcc openmp_strategies.c -o openmp_strategies -fopenmp
mpicc -g -Wall -o mpi_crout mpi_crout.c
