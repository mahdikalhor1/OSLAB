#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5

int numSamples = 50;

void printHistogram(int *hist);
int shmId;
int mainPid;
int childPid;
int *histogram;

int main(int argc, char const *argv[])
{
   if (argc == 2)
   {
       numSamples = atoi(argv[1]);
   }

   srand(time(NULL));

   if ((shmId = shmget(IPC_PRIVATE, 25 * sizeof(int), IPC_CREAT | 0666)) < 0)
   {
       printf("couldn't create the shared memory");
       exit(-1);
   }

   mainPid = getpid();
   for (int i = 0; i < NUM_THREADS; i++)
   {
       if (getpid() == mainPid)
           childPid = fork();
       else
           break;
   }
   if ((histogram = (int *)shmat(shmId, NULL, 0)) == (int *)-1)
   {
       printf("couldn't attach to the shared memory");
       exit(-1);
   }

   if (getpid() == mainPid)
   {
       for (int i = 0; i < NUM_THREADS; i++)
       {
           wait(NULL);
       }
   }
   else
   {
       int counter, random;
       for (int i = 0; i < numSamples / NUM_THREADS; i++)
       {
           counter = 0;
           for (int j = 0; j < 12; j++)
           {
               random = rand() % 101;
               if (random >= 49)
               {
                   counter++;
               }
               else
               {
                   counter--;
               }
           }
           histogram[counter + 12]++;
       }
       exit(0);
   }

   printHistogram(histogram);
   return 0;
}

void printHistogram(int *hist)
{
	int i,j;
	for (i = 0; i < 25; i++)
	{
		for(j = 0; j < hist[i]; j++){
		  printf("*");
		}
		printf("\n");
	}
}
