#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>
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

  long double e=0.0, result=0.0, sum=0.0, begin=0.0, end=0.0;
  int numThreads=0;

  if (argc == 2) {
     numThreads = atoi(argv[1]);
     omp_set_num_threads(numThreads);
     double start_time=omp_get_wtime();
     #pragma omp parallel for shared(result)
     for (int i=0; i<N; i+=1)
     {
       #pragma omp critical
       result+=((long double)pow_int(1,i)) / ((long double) fact(i));
     }
     double time = omp_get_wtime() - start_time;

     printf("Calculated Exp=%.30Lg \n", result);
     printf("Real       Exp=%.30g \n", exp(double(1)));
     printf("Elapsed time: %gs\n",time);
}

else {
  printf("Not enough arguments. Restart program");
  exit(0);
}

return 0;

}
