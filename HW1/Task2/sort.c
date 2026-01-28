#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

const int POOLSIZE = 100000;
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

        if (!thread1arg || !thread2arg) {
            free(thread1arg); free(thread2arg);
            argum left = { args.array, args.low, pivot_index - 1 };
            argum right = { args.array, pivot_index + 1, args.high };
            quicksort((void*)&left);
            quicksort((void*)&right);
            return NULL;
        }

        *thread1arg = (argum) { args.array, args.low, pivot_index - 1 };
        *thread2arg = (argum) { args.array, pivot_index + 1, args.high };

        pthread_mutex_lock(&numThreadsMutex);
        if(numOfThreads + 2 <= MAXCOUNT){
            numOfThreads += 2;
            pthread_mutex_unlock(&numThreadsMutex);

            pthread_t thread1, thread2;
            int r1 = pthread_create(&thread1, NULL, quicksort, thread1arg);
            int r2 = pthread_create(&thread2, NULL, quicksort, thread2arg);

            if (r1 == 0) pthread_join(thread1, NULL); 
            else quicksort((void*)thread1arg);
            
            if (r2 == 0) pthread_join(thread2, NULL); 
            else quicksort((void*)thread2arg);

            
            pthread_mutex_lock(&numThreadsMutex);
            if (r1 == 0) numOfThreads--;
            if (r2 == 0) numOfThreads--;
            pthread_mutex_unlock(&numThreadsMutex);
        } else {
            pthread_mutex_unlock(&numThreadsMutex);
            quicksort((void*)thread1arg);
            quicksort((void*)thread2arg);
        }
        free(thread1arg);
        free(thread2arg);
    }
    return NULL;
}

double timer() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

int main(){
    srand(time(NULL));
    double start_time, end_time;
    numOfThreads = 1;
    
    pthread_mutex_init(&numThreadsMutex, NULL);
    int *unsortedData = malloc(sizeof(int) * POOLSIZE);
    if (!unsortedData) return 1;

    for(int i = 0; i < POOLSIZE; i++){
        unsortedData[i] = rand() % 100000;
    }

    argum *args = malloc(sizeof(argum));
    if (!args) { free(unsortedData); return 1; }
    *args = (argum) {unsortedData, 0, POOLSIZE - 1};

    start_time = timer();
    pthread_t initialThread;
    if (pthread_create(&initialThread, NULL, quicksort, args) == 0) {
        pthread_join(initialThread, NULL);
    } else {
        quicksort((void*)args);
    }
    end_time = timer();

    int sorted = 1;
    for (int i = 1; i < POOLSIZE; i++) {
        if (unsortedData[i-1] > unsortedData[i]) { sorted = 0; break; }
    }
    
    printf("Sort Status: %s\n", sorted ? "SUCCESS" : "FAILED");
    printf("The execution time is %g sec\n", end_time - start_time);

    pthread_mutex_destroy(&numThreadsMutex);
    free(unsortedData);
    free(args);
    return 0;
}