# !/bin/bash
# $1 -- n
# $2 -- file
# $3 -- nThreads
# $4 -- strategy
if [[ $4 -gt 3 ]]
then
    echo "MPI"
else
    # OMP
    # ./executable fileName n strategy nThreads
    ./openmp_strategies $2 $1 $4 $3
fi
