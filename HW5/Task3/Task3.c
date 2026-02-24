#include <stdio.h>
#include <unistd.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>


#define REQ  1
#define GRANT 2
#define REL  3

void server(int rank, int num_philosophers){
   
   int forkFree[5] = {1, 1, 1, 1, 1};
   int waitingForks[5] = {0, 0, 0, 0, 0};
   
   
   
    printf("Server(rank %d) has started\n", rank);
    
int id;
MPI_Status status;
int L, R;
 for(int i = 0; i < num_philosophers * 3 * 2; i++) {
        MPI_Recv(&id, 1, MPI_INT,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);

                 if(status.MPI_TAG == REQ){
printf("Server got REQ from philosopher %d (rank %d)\n",
id, status.MPI_SOURCE);
fflush(stdout);
 L = id;
R = (id + 1) % num_philosophers;

if(forkFree[L] && forkFree[R]){
forkFree[L] = 0;
forkFree[R] = 0;



printf("Server grants permission to philosopher %d (rank %d)\n",id, status.MPI_SOURCE);
fflush(stdout);
MPI_Send(&id, 1, MPI_INT, status.MPI_SOURCE, GRANT, MPI_COMM_WORLD);


}else{waitingForks[id] = 1;
}


    }else if(status.MPI_TAG == REL){
     printf("Server got REL from philosopher %d (rank %d)\n",
 id, status.MPI_SOURCE);
 fflush(stdout);

 L = id;
R = (id + 1) % num_philosophers;
forkFree[L] = 1;
forkFree[R] = 1;

for(int j = 0; j < num_philosophers; j++){

int Lj = j;
int Rj = (j + 1) % num_philosophers;
    if(waitingForks[j] && forkFree[Lj] && forkFree[Rj]){
forkFree[Lj] = 0;
forkFree[Rj] = 0;
 waitingForks[j] = 0;
 printf("Server grants permission to philosopher %d (rank %d)\n",j, j+1);
 fflush(stdout);
MPI_Send(&j, 1, MPI_INT, j+1, GRANT, MPI_COMM_WORLD);

                 }

        
    }



}
 }
}
void philosopher(int philospher_id, int num_philosophers){

srand(time(NULL) + philospher_id * 1000);


int dummy;
MPI_Status status;


    for(int i = 0; i < 3; i++){

printf("Philosopher %d is thinking\n", philospher_id);
fflush(stdout);
sleep(rand() % 3 + 1);

printf("Philosopher %d is hungry\n", philospher_id);
fflush(stdout);
MPI_Send(&philospher_id, 1, MPI_INT, 0, REQ, MPI_COMM_WORLD);

MPI_Recv(&dummy, 1, MPI_INT, 0, GRANT, MPI_COMM_WORLD, &status);

printf("Philosopher %d is eating\n", philospher_id);
fflush(stdout);
sleep(rand() % 3 + 1);


MPI_Send(&philospher_id, 1, MPI_INT, 0, REL, MPI_COMM_WORLD);

    }
}


int main(int argc, char *argv[]) {

    int rank, size;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

int num_philosophers = size - 1;

    if(rank == 0){
        server(rank, num_philosophers);
    } else {
        philosopher(rank-1, num_philosophers);
      
    }


MPI_Finalize();
    return 0;



}


