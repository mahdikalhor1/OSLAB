#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int *count = 0;

pthread_mutex_t write_lock;

int reader_cnt = 0;

pthread_mutex_t mutex;


int shmid;

void *reader(void *r) {   

	key_t key = ftok("shm", 1);
    shmid = shmget(key, 10 * sizeof(int), 0444 | IPC_CREAT);
	count = (int *)shmat(shmid, NULL, 0);

    while(1){
    pthread_mutex_lock(&mutex);

    reader_cnt++;

    if(reader_cnt == 1) {
        pthread_mutex_lock(&write_lock);
    }

    pthread_mutex_unlock(&mutex);

    printf("Read %d, pid: %ld\n", *count, (long)getpid());

    pthread_mutex_lock(&mutex);

    reader_cnt--;

    if(reader_cnt == 0) {
        pthread_mutex_unlock(&write_lock);
    }
    pthread_mutex_unlock(&mutex);

    if(*count >9){
        break;
    }
    sleep(0.001);
    }

}


void *writer(void *w) {   
	*count = 0;

	key_t key = ftok("shm", 1);
    shmid = shmget(key, 10 * sizeof(int), 0666 | IPC_CREAT);
	count = (int *)shmat(shmid, NULL, 0);

    while (1){
        pthread_mutex_lock(&write_lock);
	    *count = *count + 1;
	    printf("Write %d, pid: %ld\n",*count,(long)getpid());
	
        pthread_mutex_unlock(&write_lock);

        if(*count >= 10)
	    	break;
        sleep(0.001);
	}

}

int main() {   
	key_t key = ftok("shm", 1);
    shmid = shmget(key, 10 * sizeof(int), 0666 | IPC_CREAT);
    count = (int *)shmat(shmid, NULL, 0);

    pthread_t wr,rd1, rd2;
    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&write_lock,NULL);

    int num1 = 1;
    int num2 = 2;

    pthread_create(&rd1, NULL, (void *)reader, (void *)&num1);
    pthread_create(&rd2, NULL, (void *)reader, (void *)&num2);
    pthread_create(&wr, NULL, (void *)writer, (void *)&num1);

    pthread_join(rd1, NULL);
    pthread_join(rd2, NULL);
    pthread_join(wr, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&write_lock);

    shmdt(count);
    return 0;
}