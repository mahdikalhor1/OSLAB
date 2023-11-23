#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>


#define PORT 8080


typedef struct {
    int client_socket;
    char username[50];
} User;

typedef struct {
    char group_id[50];
    User* user_list[51];
    int indexofusers;
} Group;

User user_list[50];
int indexofuser=0;
Group group_list[10];
int indexofgroup=0;

int register_client(int client_socket, User **current_user);
void* handle_client(void* client_socket);
void create_group(char* groupid, char* message);
void join_group(User *current_user, char* groupid, char* message);
void send_message(User *current_user, char* groupid, char* send_message, char* message);

void* handle_client(void* client_socket){
    setbuf(stdout, NULL);


    User* current_user;

    register_client(*(int*)client_socket, &current_user);


    while(1){

        char message[1024];
        char* commands[3];
        

        recv(*(int*)client_socket, message, 1024, 0);

        commands[0]=strtok(message, " ");
        int i=0;

        while(commands[i] != NULL){
            i++;
            commands[i]=strtok(NULL, " ");
        }
        
        

        if (strcmp(commands[0], "create") == 0)
        {
            create_group(commands[1], message);
            send(*(int*)client_socket, message, strlen(message), 0);
        }
        else if(strcmp(commands[0], "join") == 0)
        {
            join_group(current_user, commands[1], message);
            send(*(int*)client_socket, message, strlen(message), 0);
        }
        else if(strcmp(commands[0], "send") == 0)
        {
            send_message(current_user, commands[1], commands[2], message);
            send(*(int*)client_socket, message, strlen(message), 0);
        }
        

    }
}

int register_client(int client_socket, User **current_user){
    char username[51]="";
    recv(client_socket, username, 51, 0);

    user_list[indexofuser].client_socket=client_socket;
    strcpy(user_list[indexofuser].username, username);

    //set the current user
    *current_user= &user_list[indexofuser];
    
    indexofuser++;

    char message[100];
    sprintf(message, "you are registered with username: %s", username);
    printf("user with username: %s added.", username);
    
    send(client_socket, message, strlen(message), 0);

    return 0;
}



int main(int argc, char const *argv[]) 
{
    // char buffer[1024] = {0};

    // char *hello = "Hello from server";
    
    // creates socket file descriptor
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // host to network -- coverts the ending of the given integer
    
    const int addrlen = sizeof(address);
    // binding
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


        if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Listen on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    while(1)
    {

        int client_socket;

        // accepting client
        // accept returns client socket and fills given address and addrlen with client address information.
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("New client connected %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        

        pthread_t client_thread;
        
        pthread_create(&client_thread, NULL, handle_client, (void*)&client_socket); //creating thread for every client connected
        pthread_detach(client_thread);
    }

    // reads a buffer with maximum size 1024 from socket.
    // valread = read(client_socket, buffer, 1024);
    
    // if (valread < 0) {
    //     perror("read");
    //     exit(EXIT_FAILURE);
    // }
    
    // printf("(s = %d) %s\n", valread, buffer);
    // // writes to client socket
    // send(client_socket, hello, strlen(hello), 0);
    // printf("Hello message sent\n");
    
    return 0;
}

void create_group(char* groupid, char* message)
{
    if(indexofgroup >= sizeof(group_list)/sizeof(Group))
    {
        strcpy(message, "groups list is full!");
        return;
    }

    strcpy(group_list[indexofgroup].group_id,groupid);
    group_list[indexofgroup].indexofusers=0;
    indexofgroup++;
    strcpy(message, "group added successfully.");
}

void join_group(User *current_user, char* groupid, char* message){

    for(int i = 0 ; i < indexofgroup; i++)
    {

        if(0 == strcmp(group_list[i].group_id, groupid)){
            group_list[i].user_list[group_list[i].indexofusers]=current_user;
            group_list[i].indexofusers++;
            strcpy(message,"you are added to group successfully.");
            return;
        }
    }

    strcpy(message,"invalid groupid!");
}

void send_message(User *current_user, char* groupid, char* send_message, char* message){

    bool ismember=false;
    char sm[1024];
    sprintf(sm, "%s:%s: %s", current_user->username, groupid, send_message);

    for(int i = 0 ; i < indexofgroup; i++)
    {
        if(10 == strcmp(group_list[i].group_id, groupid)){
            
            for(int j=0; j < group_list[i].indexofusers; j++){
                if(current_user == group_list[i].user_list[j]){
                    ismember=true;
                }
            }

            if(!ismember){
                strcpy(message,"you are not member of this group!");
                return;
            }

            for(int j=0; j < group_list[i].indexofusers; j++){
                // User* user=group_list[i].user_list[j];
                // printf("%s", current_user->username);
                // printf("%s %d\n", group_list[i].user_list[j]->username,group_list[i].user_list[j]->client_socket);

                // if(current_user != group_list[i].user_list[j]){
                send(group_list[i].user_list[j]->client_socket, sm, strlen(sm), 0);
                    
                // }
            }
            strcpy(message,"message sent!");
            return;
        }
    }

    strcpy(message,"invalid groupid!");
}