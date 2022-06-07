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
    printf("%d\n", close(soc));
    printf("%d\n", unlink("./un"));
}

int main(int argc, char* args[]){
    signal(SIGINT, handle_sigint);
    atexit(handle_exit);

    char path[64] = "./un";

    soc = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}

    struct sockaddr_un server_address;
    server_address.sun_family = AF_LOCAL;
    strcpy(server_address.sun_path, path);
    if(bind(soc, (struct sockaddr*)&server_address, sizeof(sa_family_t)) == -1){printf("bind error\n"); return 1;}
    if(connect(soc, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){printf("connect error\n"); return 1;}

    char message[1024] = "Hello\n";
    send(soc, message, 1024, 0);

    char response[1024];
    recv(soc, response, 1024, 0);
    printf("%s\n", response);

    printf("%d\n", shutdown(soc, SHUT_RDWR));
    return 0;
}