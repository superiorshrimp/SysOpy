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

void handle_exit(void){
    if(shutdown(soc, SHUT_RDWR) == -1){printf("shutdown error\n"); exit(1);}
    close(soc);
}

int main(int argc, char* args[]){
    atexit(handle_exit);
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    else{printf("socket succesful\n");}

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    int port = 10000;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if(connect(soc, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){printf("connect error\n"); return 1;}

    for(int i = 0; i<10; i++){
        char* message = (char*)calloc(1024, sizeof(char));
        sprintf(message, "%d", i);
        send(soc, message, 1024, 0);
        printf("sent %s\n", message);
        free(message);
        message = (char*)calloc(1024, sizeof(char));
        recv(soc, message, 1024, 0);
        printf("received %s\n", message);
        free(message);
    }

    printf("end\n");

    return 0;
}