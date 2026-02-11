#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>


#define H 15
#define BEES 5

int honeyPot = 0;//current amount of honey in the pot
int n;  //number of bees

sem_t pot_mutex;
sem_t pot_full;
sem_t empty;


void* bee(void* arg){

int id = *(int*)arg;

while(1){


  sem_wait(&empty); //wait for the pot to be available
sem_wait(&pot_mutex);
honeyPot++;   //add honey to the pot
printf("Bee %d added honey. Current honey in the pot: %d\n", id, honeyPot); 
if(honeyPot == H){  //if the pot is full, wake up the bear
    sem_post(&pot_full);
}


sem_post(&pot_mutex);//release the pot for other bees


 usleep((rand() % 300 + 200) * 1000);
    



}

return NULL;

}


void* bear(void* arg){

while(1){

sem_wait(&pot_full);
sem_wait(&pot_mutex);

printf("Current honey in the pot: %d,Bear is eating all of the honey. \n", honeyPot);
honeyPot = 0;

sem_post(&pot_mutex);


for(int i = 0; i < H; i++){   //make the pot available for the bees again
sem_post(&empty);

}
 sleep(1);

}

return NULL;
}













int main(int argc, char* argv[]){


sem_init(&pot_mutex, 0, 1);
sem_init(&pot_full, 0, 0);
sem_init(&empty, 0, H);



pthread_t bear_thread;
    pthread_t bee_threads[BEES];
    int ids[BEES];


    pthread_create(&bear_thread, NULL, bear, NULL);

for(int i = 0; i < BEES; i++){

ids[i] = i + 1;
pthread_create(&bee_threads[i], NULL, bee, &ids[i]);


}


pthread_join(bear_thread, NULL);

return 0;   

}

