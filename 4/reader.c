#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main()
{
	// ftok to generate unique key
	key_t key = ftok("shmfile", 65);

	// shmget returns an identifier in shmid
	int shmid = shmget(key, 100, 0666 | IPC_CREAT);

	// shmat to attach to shared memory
	char* str = (char*)shmat(shmid, NULL, 0);

    printf("reading value from memory:\n %s\n", str);
	// detach from shared memory
	shmdt(str);

	// destroy the shared memory
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
