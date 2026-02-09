#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>


int honeyPot = 0;//current amount of honey in the pot
int H;  //capacity of the honey pot
int n;  //number of bees

sem_t pot_mutex;
sem_t pot_full;
sem_t pot_available;


void* bee(void* arg){

int id = *(int*)arg;

while(1){


  sem_wait(&pot_available); //wait for the pot to be available
sem_wait(&pot_mutex);
honeyPot++;   //add honey to the pot

if(honeyPot == H){  //if the pot is full, wake up the bear
    sem_post(&pot_full);
}

else{     //if the pot is not full, make it available for other bees
    sem_post(&pot_available);
}
sem_post(&pot_mutex);



    



}

return NULL;

}


void* bear(void* arg){

while(1){

sem_wait(&pot_full);
printf("Bear is eating honey. Current honey in the pot: %d\n", honeyPot);
honeyPot = 0;
sem_post(&pot_available);


}

}


int main(int argc, char* argv[]){





pthread_t bear_thread;
    pthread_t bee_threads[n];
    int ids[n];




return 0;

}

