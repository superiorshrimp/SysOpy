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

void handle_sigint(int signum){
    exit(0);
}

void handle_exit(void){
    close(soc);
}

int main(int argc, char* args[]){
    signal(SIGINT, handle_sigint);
    atexit(handle_exit);

    soc = socket(AF_INET, SOCK_DGRAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    int port = 10000;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if(connect(soc, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){printf("connect error\n"); return 1;}

    send(soc, "Hello\n", 1024, 0);

    char msg[1024];
    recv(soc, msg, 1024, 0);
    printf("received %s\n", msg);

    shutdown(soc, SHUT_RDWR);
    return 0;
}