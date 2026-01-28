#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

const int POOLSIZE = 200000;

void swap(int* array, int left, int right){
    int temp = array[right];
    array[right] = array[left];
    array[left] = temp;
}

int partition(int* array, int low, int high){
    int pivot = array[low];
    int leftwall = low;
    int i = low + 1;
    while(i <= high){
        if(array[i] < pivot){
            leftwall++;
            swap(array, i, leftwall);
        }
        i++;
    }
    
    swap(array, low, leftwall);

    return leftwall;
}



void quicksort(int* array, int low, int high){
    if(low < high){
        int pivot_index = partition(array, low, high);
        quicksort(array, low, pivot_index - 1);
        quicksort(array, pivot_index + 1, high);
    }

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
    int *unsortedData = malloc(sizeof(int) * POOLSIZE);
    int i = 0;
    for(i = 0; i < POOLSIZE; i++){
        unsortedData[i] = rand() % 10;
       
    }
    start_time = timer();
    quicksort(unsortedData, 0, POOLSIZE -1);
    end_time = timer();
    printf("The execution time is %lf sec" ,end_time - start_time);
    return 0;
}