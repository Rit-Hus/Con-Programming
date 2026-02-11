#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BABYBIRDS 5
#define FOOD 12
sem_t get_food;
sem_t find_food;
sem_t wait_food;
int food;

void* babyBird(void* args){
    int id = *(int*) args;

    while(1){
        // Only one bird can check the dish at a time
        sem_wait(&get_food);
        
        printf("Supplies left: %d\n", food);
        printf("Bird %d looking for sustenance\n", id);

        if(food == 0){
            printf("Bird %d calls for the father!\n", id);
            sem_post(&find_food); // Wake up father
            sem_wait(&wait_food); 
           
        }

        // food consumed
        food--;
        printf("Bird %d consumes sustenance. Remaining: %d\n", id, food);

        sem_post(&get_food);

        printf("Bird %d slumbers...\n", id);
        sleep(rand() % 3 + 1);
    }
}

void* papaBird(void* args){
    while(1){
        // Wait until a baby chirps
        sem_wait(&find_food);
        
        printf("Father: 'I must gather more food!'\n");
        sleep(rand() % 3 + 1); 
        
        food = FOOD; 
        printf("Father: 'The supplies are refilled!'\n");

        sem_post(&wait_food);
    }
}




int main() {
    pthread_t babyBirds[BABYBIRDS];
    pthread_t papa; 
    food = FOOD;

    // Semaphores
    sem_init(&get_food, 0, 1);
    sem_init(&find_food, 0, 0);
    sem_init(&wait_food, 0, 0);

    
    if (pthread_create(&papa, NULL, papaBird, NULL) != 0) {
        perror("Failed to create papa bird");
        return 1;
    }

    // Create Baby Birds
    int babyBirdIds[BABYBIRDS];
    for (int i = 1; i < BABYBIRDS; i++) {
        babyBirdIds[i] = i ; // Start IDs at 1 for better logs
        if (pthread_create(&babyBirds[i], NULL, babyBird, &babyBirdIds[i]) != 0) {
            perror("Failed to create baby bird");
        }
    }

    // KEEP MAIN ALIVE
    pthread_join(papa, NULL);

    // this will never be reached
    sem_destroy(&get_food);
    sem_destroy(&find_food);
    sem_destroy(&wait_food);

    return 0;
}
