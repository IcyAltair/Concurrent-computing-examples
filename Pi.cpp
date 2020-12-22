#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
volatile long double pi = 0.0;
pthread_mutex_t piLock;
long double intervals;
int numThreads;

void *computePI(void *id)
{
    long double x,width,localSum = 0;
    int i,threadID = *((int*)id);

    width = 1.0 / intervals;

    for(i = threadID ; i < intervals; i += numThreads) {
        x = (i+0.5) * width;
        localSum += 4.0 / (1.0 + x*x);
    }

    localSum *= width;

    pthread_mutex_lock(&piLock);
    pi += localSum;
    pthread_mutex_unlock(&piLock);

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t *threads;
    void *retval;
    int *threadID;
    int i;

   if (argc == 3) {
      intervals = atoi(argv[1]);
      numThreads = atoi(argv[2]);
      threads = (pthread_t*)malloc(numThreads*sizeof(pthread_t));
      threadID = (int*)malloc(numThreads*sizeof(int));
      pthread_mutex_init(&piLock, NULL);

      clock_t begin=clock();
      for (i = 0; i < numThreads; i++) {
        threadID[i] = i;
        pthread_create(&threads[i], NULL, computePI, threadID+i);
      }
      for (i = 0; i < numThreads; i++) {
         pthread_join(threads[i], &retval);
      }
      clock_t end=clock();
      printf("Calculated PI=%.30Lg \n", pi);
      printf("Real       PI=%.30g \n", M_PI);
      printf("Elapsed time: %gs\n",(double)(end - begin) / CLOCKS_PER_SEC);

    } else {
      printf("Not enough arguments. Restart program");
      exit(0);
    }

    return 0;
}
