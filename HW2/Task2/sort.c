#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

const int THRESHOLD = 1000;  
const int POOLSIZE = 500000; // Fixed space in number
const int MAXCOUNT = 8;

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

// Changed to void as discussed - no return needed
void quicksort(int* array, int low, int high){
   if (high > low ) {
        int pivot_index = partition(array, low, high);
        if (high - low > THRESHOLD) {
            #pragma omp task
            quicksort(array, low, pivot_index - 1);

            #pragma omp task
            quicksort(array, pivot_index + 1, high);

            #pragma omp taskwait
        } else {
            quicksort(array, low, pivot_index - 1);
            quicksort(array, pivot_index + 1, high);
        }
    }
}

int main(){
    double times[5];
    int *unsortedData = malloc(sizeof(int) * POOLSIZE);
    
    int run, j, i, k;

    omp_set_num_threads(MAXCOUNT);
    printf("Testing with %d processors and %d elements...\n", MAXCOUNT, POOLSIZE);

    for (run = 0; run < 5; run++) {
        for (k = 0; k < POOLSIZE; k++) {
            unsortedData[k] = rand() % 100000;
        }

        double start_time = omp_get_wtime();

        #pragma omp parallel
        {
            #pragma omp single
            quicksort(unsortedData, 0, POOLSIZE - 1);
        }

        times[run] = omp_get_wtime() - start_time;
        printf("Run %d: %g sec\n", run + 1, times[run]);
    }

    // Sort the 5 times for median
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4 - i; j++) {
            if (times[j] > times[j + 1]) {
                double temp = times[j];
                times[j] = times[j + 1];
                times[j + 1] = temp;
            }
        }
    }
    
    printf("\nMEDIAN EXECUTION TIME: %g sec\n", times[2]);
    free(unsortedData);
    return 0;
}