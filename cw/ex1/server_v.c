#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include "consts.h"

int q;

int clients[CLIENTS];
int active[CLIENTS];

void handle_exit(void){
    msgctl(q, IPC_RMID, NULL);
}

void handle_init(char* message){
    int i = -1;
    for(i = 0; i<CLIENTS; i++){
        if(active[i] == 0){
            active[i] = 1;
            clients[i] = atoi(message);
            struct response{long type; char content[LENGTH];} response;
            response.type = RSP;
            sprintf(response.content, "%d", i);
            msgsnd(clients[i], &response, LENGTH, IPC_NOWAIT);
            break;
        }
    }
}

void handle_msg(char* message){
    char name[2];
    name[0] = message[0];
    name[1] = '\0';
    int id = atoi(name);
    if(active[id] != 1){
        return;
    }
    struct response{long type; char content[LENGTH];} response;
    response.type = RSP;
    strcpy(response.content, "i got your message\n");
    printf("%d\n", clients[id]);
    msgsnd(clients[id], &response, LENGTH, IPC_NOWAIT);
}

void ex(int signum){exit(0);}

int main(int argc, char* args[]){
    signal(SIGINT, ex);
    atexit(handle_exit);
    for(int i = 0; i<CLIENTS; i++){
        active[i] = 0;
        clients[i] = -1;
    }

    q = msgget(SERVER_KEY, IPC_CREAT | 0666);
    if(q == -1){
        q = msgget(SERVER_KEY, 0666);
        if(q != -1){
            q = msgget(SERVER_KEY, IPC_CREAT | 0666);
        } else{printf("msgget error\n"); return 1;}
    }

    while(1){
        struct message{long type; char content[LENGTH];} message;
        msgrcv(q, &message, LENGTH, 0, MSG_NOERROR);
        switch(message.type){
            case STOP:
                printf("stop\n");
                printf("%s\n", message.content);
                break;
            case INIT:
                printf("new client %d\n", atoi(message.content));
                handle_init(message.content);
                break;
            case MSG:
                printf("message: %s\n", message.content);
                handle_msg(message.content);
                break;
            default:
                printf("wrong type\n");
        }
    }

    return 0;
}