#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

void print_menu(){

    printf("%s\n%s\n%s\n%s\n%s\n\n", "create groupid",
     "join groupid", "send groupid message",
     "leave groupid", "quit");
}

void* get_message(void* sock){

    setbuf(stdout, NULL);

    while(1){
        char buffer[1024]="";
        recv(*(int*)sock, buffer, 1024, 0);
        printf("%s\n", buffer);
    }
}

void send_message(int sock){

    size_t size=1024;

    while(1){

        char* message=NULL;
        // strcpy(message[0], "");
        // strcpy(message[1], "");
        // strcpy(message[2], "");

        printf("Enter an input:(help)\n");
        getline(&message, &(size), stdin);
        // scanf("%s");

        if(strcmp(message, "help\n") == 0){
            print_menu();
        }
        else if(strcmp(message, "quit\n") == 0){
            printf("exited.");
            return;
        }
        else{
            send(sock,message , 1024, 0);
        }
        

    }
}

int main(int argc, char const *argv[]) 
{
    int sock = 0, valread;

    struct sockaddr_in serv_addr;



    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // sets all memory cells to zero
    memset(&serv_addr, '0', sizeof(serv_addr));

    // sets port and address family
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // connects to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    char username[51];
    printf("enter a username:");
    scanf("%s", username);
    send(sock, username, strlen(username), 0);

    pthread_t thread;
        
    pthread_create(&thread, NULL, get_message, (void*)&sock);

    send_message(sock);
    return 0;
}