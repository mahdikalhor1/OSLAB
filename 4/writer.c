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

    sprintf(str, "\"some data...\"");

    printf("\"some data...\" is writed into memory\n");

	// detach from shared memory
	shmdt(str);

	return 0;
}
