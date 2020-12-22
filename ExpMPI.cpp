#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define N 10

long sqr(long n) {
  return n*n;
}

long fact(long n) {
  long i, prod;
  for (i=1, prod=1; i<=n; prod=prod*i, i++) { };
  return prod;
}

long pow_int(long z, long n) {
  if (n==0)
    return 1;
  if (n==1)
    return z;
  if (n%2==0)
    return sqr(pow_int(z,n/2));
  else
    return z*sqr(pow_int(z,n/2));
}

int main (int argc, char **argv) {

  int rank, size, i;
  long double e=0.0, result=0.0, sum=0.0, begin=0.0, end=0.0;

  MPI_Init (&argc, &argv);


  MPI_Comm_rank (MPI_COMM_WORLD, &rank);


  MPI_Comm_size (MPI_COMM_WORLD, &size);


  MPI_Barrier(MPI_COMM_WORLD);
  begin = MPI_Wtime();


  for (i=rank; i<N; i+=size)
  {
      result+=((long double)pow_int(1,i)) / ((long double) fact(i));
  }


  MPI_Reduce(&result, &sum, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();


  if (rank==0)
  {
      e=sum;
      printf("np=%2d;    Time=%Lfs;    Exp=%.30Lg\n", size, end-begin, e);
  }

  MPI_Finalize();

  return 0;
}
