#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

int soc;
int client = -1;

void handle_exit(void){
    if(shutdown(soc, SHUT_RDWR) == -1){printf("shutdown error\n"); exit(1);}
    close(soc);
}

void sigpipe_handle(int signum){exit(0);}

void* connect_handler(void* args){
    listen(soc, 1);
    while(1){
        client = accept(soc, NULL, NULL);
        if(client != -1){
            printf("connected %d\n", client);
        }
    }

    return NULL;
}

int main(int argc, char* args[]){
    signal(SIGPIPE, sigpipe_handle);
    atexit(handle_exit);
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    else{printf("socket succesful\n");}

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(10000);
    char* ip = "0.0.0.0";
    server_address.sin_addr.s_addr = inet_addr(ip); 
    if(bind(soc, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){printf("bind error\n"); return 1;}
    else{printf("bind succesful\n");}

    pthread_t connect_thread;
    pthread_create(&connect_thread, NULL, connect_handler, NULL);
    
    while(1){
        if(client != -1){
            char* message = (char*)calloc(1024, sizeof(char));
            recv(client, message, 1024, 0);
            printf("received %s\n", message);
            int val = atoi(message);
            free(message);
            message = (char*)calloc(1024, sizeof(char));
            sprintf(message, "%d", val*val);
            send(client, message, 1024, 0);
            printf("sent %s\n", message);
            free(message);
        }
    }

    pthread_join(connect_thread, NULL);

    return 0;
}