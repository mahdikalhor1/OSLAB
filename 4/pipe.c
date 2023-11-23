#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


#define READ_END 0
#define WRITE_END 1


int main(void)
{

    int fd1[2], fd2[2];

    pid_t childpid;

    if (pipe(fd1) == -1 || pipe(fd2) == -1){
        printf("creating pipe failed.");
        return 0;
    }


    childpid = fork();


    if (childpid == -1) {
        printf("fork failed.");
        return 0;
    }


    if(childpid != 0) {
        
        char* message="MeSSage FrOm PArent.";

        close(fd1[READ_END]);
        close(fd2[WRITE_END]);

        write(fd1[WRITE_END], message, 21);  
        
        int status;
	    wait(&status);

        char result[20];
        read(fd2[READ_END], result, sizeof(result));

        printf("recieved message: %s\n", result);

        close(fd2[READ_END]);
        close(fd1[WRITE_END]);
    }
    else 
    {

        close(fd2[READ_END]);
        close(fd1[WRITE_END]);


        char message[30];

        read(fd1[READ_END], message, sizeof(message));
        

        for(size_t i=0; i < strlen(message) ; i++)
        {
            if(message[i] >= 'a' && message[i] <= 'z')
            {
                message[i]-=32;
            }
            else if(message[i] >= 'A' && message[i] <= 'Z'){
                message[i]+=32;
            }
        }


        write(fd2[WRITE_END], message, sizeof(message));

        close(fd1[READ_END]);
        close(fd2[WRITE_END]); 
    }

    return 0;
}