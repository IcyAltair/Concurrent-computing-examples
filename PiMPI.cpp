//https://en.wikipedia.org/wiki/Trapezoidal_rule
#include <stdio.h>
#include <mpi.h>

#define N 1E7
#define d 1E-7
#define d2 1E-14

int main (int argc, char* argv[])
{
    int rank, size, i;
    long double pi=0.0, result=0.0, sum=0.0, begin=0.0, end=0.0, x2;

    MPI_Init (&argc, &argv);


    MPI_Comm_rank (MPI_COMM_WORLD, &rank);


    MPI_Comm_size (MPI_COMM_WORLD, &size);


    MPI_Barrier(MPI_COMM_WORLD);
    begin = MPI_Wtime();


    for (i=rank; i<N; i+=size)
    {
        x2=d2*i*i;
        result+=1.0/(1.0+x2);
    }


    MPI_Reduce(&result, &sum, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();


    if (rank==0)
    {
        pi=4*d*sum;
        printf("np=%2d;    Time=%Lfs;    PI=%.30Lg\n", size, end-begin, pi);
    }

    MPI_Finalize();

    return 0;
}
