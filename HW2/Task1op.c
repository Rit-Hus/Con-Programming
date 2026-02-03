/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>
#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */




int numWorkers;
int size; 
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *);
double start_time, end_time;

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j;
  long long total = 0;
srand(time(NULL));
  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
    //  printf("[ ");
	  for (j = 0; j < size; j++) {
      matrix[i][j] = rand()%99;
      //	  printf(" %d", matrix[i][j]);
	  }
	  //	  printf(" ]\n");
  }

int globalMin = INT_MAX,  globalMinRow = 0, globalMinCol = 0;
int globalMax = INT_MIN, globalMaxRow = 0, globalMaxCol = 0;

  start_time = omp_get_wtime();
  #pragma omp parallel 
  {
int localMin = matrix[0][0];
int localMinRow = 0, localMinCol = 0;
int localMax = matrix[0][0];
int localMaxRow = 0, localMaxCol = 0;



#pragma omp  for reduction (+:total) private(j)
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++){
      total += matrix[i][j];
int v = matrix[i][j];

      
    
    
    if(v < localMin) {
    localMin = v;
    localMinRow = i;
    localMinCol = j;
}

if(v > localMax) {
    localMax = v;
    localMaxRow = i;
    localMaxCol = j;
}





    }




#pragma omp critical
{

if(localMin < globalMin) {
    globalMin = localMin;
    globalMinRow = localMinRow;
    globalMinCol = localMinCol;
}
if (localMax > globalMax) {
    globalMax = localMax;
    globalMaxRow = localMaxRow;
    globalMaxCol = localMaxCol;
}
    
    
    
    
    
    
    
    
    
    
    
    }

 }

end_time = omp_get_wtime();

  printf("the total is %lld\n", total);
  printf("it took %g seconds\n", end_time - start_time);
  printf("Global Min: %d at (%d, %d)\n", globalMin, globalMinRow, globalMinCol);
  printf("Global Max: %d at (%d, %d)\n", globalMax, globalMaxRow, globalMaxCol);



}
// implicit barrier

 






 
 