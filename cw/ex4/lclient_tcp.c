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
    unlink("./un");
}

int main(int argc, char* args[]){
    signal(SIGINT, handle_sigint);
    atexit(handle_exit);

    char path[64] = "./un";

    soc = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(soc == -1){printf("socket error"); return 1;}

    struct sockaddr_un server_address;
    server_address.sun_family = AF_LOCAL;
    strcpy(server_address.sun_path, path);
    socklen_t len = sizeof(server_address);
    if(connect(soc, (struct sockaddr*)&server_address, len) == -1){printf("connect error"); return 1;}
    printf("connected\n");

    char* message = (char*)calloc(1024, sizeof(char));
    strcat(message, "Hello!\n");
    send(soc, message, 1024, 0);
    printf("%s\n", message);
    free(message);

    shutdown(soc, SHUT_RDWR);
    return 0;
}