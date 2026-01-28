#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

const int POOLSIZE = 200000;
const int MAXCOUNT = 4;

volatile int numOfThreads;
pthread_mutex_t numThreadsMutex;

typedef struct {
    int* array;
    int low;
    int high;
} argum;

void swap(int* array, int left, int right){
    int temp = array[right];
    array[right] = array[left];
    array[left] = temp;
}

int partition(int* array, int low, int high){
    int pivot = array[low];
    int wall = low;
    int i = low + 1;
    while(i <= high){
        if(array[i] < pivot){
            wall++;
            swap(array, i, wall);
        }
        i++;
    }
    
    swap(array, low, wall);

    return wall;
}



void* quicksort(void* arg){
 argum* parg = (argum*) arg;
    argum args = *parg;
    if(args.low < args.high){
        int pivot_index = partition(args.array, args.low, args.high);

        argum *thread1arg = malloc(sizeof(argum));
        argum *thread2arg = malloc(sizeof(argum));

        *thread1arg = (argum) { args.array, args.low, pivot_index - 1 };
        *thread2arg = (argum) { args.array, pivot_index + 1, args.high };

        pthread_mutex_lock(&numThreadsMutex);
        if(numOfThreads < MAXCOUNT){
            numOfThreads += 2;
            pthread_mutex_unlock(&numThreadsMutex);

            pthread_t thread1;
            pthread_t thread2;

            pthread_create(&thread1, NULL, quicksort, thread1arg);
            pthread_create(&thread2, NULL, quicksort, thread2arg);

            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);

            pthread_mutex_lock(&numThreadsMutex);
            numOfThreads -= 2;
            pthread_mutex_unlock(&numThreadsMutex);
        } else {
            pthread_mutex_unlock(&numThreadsMutex);
            quicksort(thread1arg);
            quicksort(thread2arg);
        }
        free(thread1arg);
        free(thread2arg);
    }
    return NULL;
}

double timer() {
    static bool timestart = false;
    static struct timeval start;
    struct timeval end;
    if( !timestart )
    {
        gettimeofday( &start, NULL );
        timestart = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

int main(){
double start_time, end_time;
    numOfThreads = 1;
    
    pthread_mutex_init(&numThreadsMutex, NULL);
    int *unsortedData = malloc(sizeof(int) * POOLSIZE);
    int i;
    for(i = 0; i < POOLSIZE; i++){
        unsortedData[i] = rand() % 10;
       // printf("%d: %d\n", i, unsortedData[i]);
    }

    argum *args = malloc(sizeof(argum));
    *args = (argum) {unsortedData, 0, POOLSIZE - 1};

    start_time = timer();
    pthread_t initialThread;
    pthread_create(&initialThread, NULL, quicksort, args);
    pthread_join(initialThread, NULL);
    end_time = timer();

    printf("The execution time is %g sec\n", end_time - start_time);

    pthread_mutex_destroy(&numThreadsMutex);
    free(unsortedData);
    free(args);

    return 0;
}