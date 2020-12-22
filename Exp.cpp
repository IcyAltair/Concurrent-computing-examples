#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define SIZE 50


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

typedef struct sum_args{
  int id;
  int n;
  int sum;
} sum_args;

long double factors[SIZE];
long double sum[SIZE]={0};
long double result=0;
int part = 0;

pthread_mutex_t lock;

void* sum_array(void* args){

  sum_args *arg=(sum_args*) args;
  int n=arg->n;
  int id=arg->id;
  int max=SIZE/n;
  int thread_part=part++;
  for(int i=thread_part*max;i<(thread_part+1)*max;i++)
      sum[thread_part]+=factors[i];
  //printf("Hello, I'm thead %d of %d and my sum is %Lg\n", id ,n,sum[thread_part]);
  pthread_mutex_lock(&lock);
  result+=sum[thread_part];
  pthread_mutex_unlock(&lock);
  return NULL;

}


int main (int argc, char **argv) {

  if (argc!=2){
      printf("Not enough arguments. Must be 2. Restart program\n");
      exit(0);
    }
  int n=atoi(argv[1]);
  if (SIZE%n!=0 || n>SIZE){
    printf("Quantity of threads must be one of dividers of %d. Restart program with one of these arguments:\n",SIZE);
    for (int i = 1; i <= SIZE; i++){
       if (SIZE % i == 0)
           printf("%d\n",i);
         }
    exit(0);
  }
  long double e=0.0;
  for (int i=0; i<SIZE; i++)
  {
      factors[i]=((long double)pow_int(1,i)) / ((long double) fact(i));
      //printf("%Lg\n", factors[i]);
  }
  printf("Range of taylor series is %d\n",SIZE);
  sum_args args[n];
  for(int i=0;i<n;i++){
    args[i].id=i;
    args[i].n=n;
  }


  pthread_t threads[n];
  pthread_mutex_init(&lock, NULL);

  clock_t begin=clock();
  for (int i = 0; i < n; i++)
          pthread_create(&threads[i], NULL, sum_array, (void*)&args[i]);
  for (int i = 0; i < n; i++)
          pthread_join(threads[i], NULL);
  clock_t end=clock();


  printf("Calculated Exp=%.30Lg \n", result);
  printf("Real       Exp=%.30g \n", exp(double(1)));
  printf("Elapsed time: %gs\n",(double)(end - begin) / CLOCKS_PER_SEC);




  return 0;
}
