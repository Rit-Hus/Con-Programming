/* matrix summation using pthreads

   features: uses a barrier; the Worker[0] computes
             the total sum from partial sums computed by Workers
             and prints the total sum to the standard output

   usage under Linux:
     gcc matrixSum.c -lpthread
     a.out size numWorkers

*/
#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <stdint.h>

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 10   /* maximum number of workers */
pthread_mutex_t taskLock;
pthread_mutex_t resultLock;

int nextRow = 0;
 /* mutex lock for result arrays */
int numWorkers;           /* number of workers */ 


long long globalSum = 0;
int globalMin, globalMinRow, globalMinCol;
int globalMax, globalMaxRow, globalMaxCol;

/* timer */
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time; /* start and end times */
int size, stripSize;  /* assume size is multiple of numWorkers */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];


srand(time(NULL));


  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&taskLock, NULL);
    pthread_mutex_init(&resultLock, NULL);


  globalSum = 0;    
  
  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
          matrix[i][j] = rand()%99;
	  }
  }

  /* initialize global sum */
globalMax = matrix[0][0]; globalMaxRow = 0; globalMaxCol = 0; 
  globalMin = matrix[0][0]; globalMinRow = 0; globalMinCol = 0;

  /* print the matrix */
#ifdef DEBUG
  for (i = 0; i < size; i++) {
	  printf("[ ");
	  for (j = 0; j < size; j++) {
	    printf(" %d", matrix[i][j]);
	  }
	  printf(" ]\n");
  }
#endif

  /* do the parallel work: create the workers */
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *)(intptr_t) l);

    
  for (l = 0; l < numWorkers; l++)
    pthread_join(workerid[l], NULL);
  end_time = read_timer();

  /* print results */
  printf("The total sum is %lld\n", globalSum);
  printf("The minimum value is %d at (%d, %d)\n", globalMin, globalMinRow, globalMinCol);
  printf("The maximum value is %d at (%d, %d)\n", globalMax, globalMaxRow, globalMaxCol);
  printf("The execution time is %g sec\n", end_time - start_time);

  return 0;
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg) {
  long myid = (long)(intptr_t) arg;
  
long long localSum = 0;
bool hasWork = false;

int localMin = 0; int localMinRow = -1; int localMinCol = -1;
int localMax = 0; int localMaxRow = -1; int localMaxCol = -1;

// process rows until there is no more work
while(1){
int row;
pthread_mutex_lock(&taskLock);
row = nextRow++;
pthread_mutex_unlock(&taskLock);

if(row >= size) break;

for(int j = 0; j< size; j++){

int v = matrix[row][j];
localSum += v;

if(!hasWork){
localMin= localMax = v;
localMinRow = localMaxRow = row;
localMinCol = localMaxCol = j;
hasWork = true;

}else{

if(v < localMin){localMin = v; localMinRow = row; localMinCol = j;}
if (v > localMax){localMax = v; localMaxRow = row; localMaxCol = j;

}
}
}

// merge local results to global results(one lock)


}

pthread_mutex_lock(&resultLock);
globalSum += localSum;

if(hasWork){

if(localMin < globalMin){

globalMin = localMin;
globalMinRow = localMinRow;
globalMinCol = localMinCol;

}


if(localMax > globalMax){

globalMax = localMax;
globalMaxRow = localMaxRow;
globalMaxCol = localMaxCol;

}
}


pthread_mutex_unlock(&resultLock);



return NULL;
}
