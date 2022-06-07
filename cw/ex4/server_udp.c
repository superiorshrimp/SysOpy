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
struct sockaddr client_address;

void handle_exit(void){
    close(soc);
}

int main(int argc, char* args[]){
    atexit(handle_exit);
    soc = socket(AF_INET, SOCK_DGRAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    else{printf("socket succesful\n");}

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(10000);
    char* ip = "0.0.0.0";
    server_address.sin_addr.s_addr = inet_addr(ip); 
    if(bind(soc, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){printf("bind error\n"); return 1;}
    else{printf("bind succesful\n");}

    socklen_t len = sizeof(client_address);
    
    char* message = (char*)calloc(1024, sizeof(char));
    recvfrom(soc, message, 1024, 0, (struct sockaddr*)&client_address, &len);
    printf("received %s\n", message);
    free(message);
    message = (char*)calloc(1024, sizeof(char));
    strcat(message, "resp\n");
    sendto(soc, message, 1024, 0, (struct sockaddr*)&client_address, len);
    printf("sent %s\n", message);
    free(message);
    

    return 0;
}