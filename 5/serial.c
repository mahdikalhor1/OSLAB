#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int numSamples = 50;

void printHistogram(int histogram[]);

int main(int argc, char const *argv[])
{
   if (argc == 2) {
       numSamples = atoi(argv[1]);
   }

   srand(time(NULL));

   int histogram[25] = {0};
   int count = 0;
   int randNum;

   int i = 0;
   while (i < numSamples)
   {
       count = 0;
       int j = 0;
       while (j < 12)
       {
           randNum = rand() % 101;
           if (randNum >= 49)
           {
               count++;
           }
           else
           {
               count--;
           }
           j++;
       }
       histogram[count + 12]++;
       i++;
   }

   printHistogram(histogram);

   return 0;
}

void printHistogram(int *hist)
{
	int i,j;
	for (i = 0; i < 25; i++)
	{
		for(j = 0; j < hist[i]; j++)
		{
		  printf("*");
		}
		printf("\n");
	}
}
