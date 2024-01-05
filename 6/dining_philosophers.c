#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t chopsticks[5];


void *philosoph_handler (void* args)
{
  int id = *((int*)args);

  int next_id = 0;
    if (id < 4) {
    	next_id = id + 1;
    }
  
  int possible1;
  int possible2;

  printf("Philosopher[%d] is Thinking... \n", id);
  sleep(0.05);

  while(1){
  	
    possible1 = pthread_mutex_trylock(&chopsticks[id]);
    possible2 = pthread_mutex_trylock(&chopsticks[next_id]);

    if (possible1 == 0 && possible2 == 0){
    	
    	printf("Philosopher[%d] is Eating by chopstick[%d] and chopstick[%d]...\n", id, id, next_id);
  		sleep(0.05);
  		printf("Philosopher[%d] finished eating!\n", id);
 			pthread_mutex_unlock(&chopsticks[id]);
 			pthread_mutex_unlock(&chopsticks[next_id]);
 			break;
    }
    
    if (possible1 == 0) {
    		pthread_mutex_unlock(&chopsticks[id]);
    }
	  if (possible2 == 0) {
		    pthread_mutex_unlock(&chopsticks[next_id]);}
	  }
}

void main() {
  pthread_t philosophs[5];
  int ids[5];

  for (int i = 0; i < 5; i++) {
    ids[i] = i;
    pthread_mutex_init(&chopsticks[i], NULL);
  }

  for (int i = 0; i < 5; i++) {
    pthread_create(&philosophs[i], NULL, philosoph_handler, &ids[i]);
  }

  for (int i = 0; i < 5; i++) {
    pthread_join(philosophs[i], NULL);
  }

}

