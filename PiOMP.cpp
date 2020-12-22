#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define N 10000000
#define d 1E-7
#define d2 1E-14

int main(int argc, char **argv)
{
  long double pi=0.0, result=0.0, sum=0.0, begin=0.0, end=0.0, x2;
  int numThreads=0;
   if (argc == 2) {
      numThreads = atoi(argv[1]);
      omp_set_num_threads(numThreads);
      double start_time=omp_get_wtime();
      #pragma omp parallel for shared(result)
      for (int i=0; i<N; i+=1)
      {
          x2=d2*i*i;
          #pragma omp critical
          result+=1.0/(1.0+x2);

      }

      double time = omp_get_wtime() - start_time;
      printf("Calculated PI=%.30Lg \n", pi=4*d*result);
      printf("Real       PI=%.30g \n", M_PI);
      printf("Elapsed time: %gs\n",time);

    } else {
      printf("Not enough arguments. Restart program");
      exit(0);
    }

    return 0;
}
