# !/bin/bash
# $1 -- n
# $2 -- file
# $3 -- nThreads
# $4 -- strategy
if [[ $4 -gt 3 ]]
then
    # MPI 
    time mpiexec -n $3 ./mpi_crout $2 $1
else
    # OMP
    # ./executable fileName n strategy nThreads
    time ./openmp_strategies $2 $1 $4 $3
fi
python3 format_checker.py $2 "output_L_$4_$3.txt" "output_U_$4_$3.txt"
