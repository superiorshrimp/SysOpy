#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "consts.h"

int q;

void client_shutdown(int signum){
    msgctl(q, IPC_RMID, NULL);
    exit(0);
}

void read_one(char* content){

}

void read_all(char* content){
    printf("received message from: %c\n", content[0]);
    char* msg = (char*)calloc(strlen(content)+1, sizeof(char));
    strncpy(msg, content+1, strlen(content)-1);
    printf("content: %s", msg);
}

int main(int argc, char* args[]){
    signal(SIGINT, client_shutdown);
    srand(time(NULL));
    int srv = msgget(SERVER_KEY, 0);
    if(srv == -1){
        printf("failed to open server's queue\n");
        return 1;
    }
    key_t key = ftok("./client.c", rand() % 2048);
    q = msgget(key, 0666|IPC_CREAT);
    if(q == -1){
        printf("client msgget error!\n");
        return 1;
    }
    printf("Client ready with id: %d\n", q);
    struct message{long type; char content[LENGTH+1];} message;
    message.type = INIT;
    sprintf(message.content, "%d", q);
    msgsnd(srv, &message, LENGTH, IPC_NOWAIT);
    printf("sent init request to server\n");
    msgrcv(q, &message, LENGTH, RESPONSE, 0);
    char* ptr;
    int id = (int)strtol(message.content, &ptr, 10);
    if(id < 0){return 1;}
    printf("received id %d from server\n", id);

    while(1){
        struct message{long type; char content[LENGTH+1];} message;
        if(msgrcv(srv, &message, LENGTH, 0, IPC_NOWAIT) == -1){
            if(errno != ENOMSG){
                printf("Error receiving message: %s\n", strerror(errno));
            }
        }
        else{
            if(message.type == ONE){
                read_one(message.content);
            }
            else if(message.type == ALL){
                printf("rcvd\n");
                read_all(message.content);
            }
        }
        char* cmd = (char*)calloc(LENGTH, sizeof(char));
        char* cmp = (char*)calloc(5, sizeof(char));
        fgets(cmd, LENGTH, stdin);
        cmd[strcspn(cmd, "\n")] = 0; //delete newline at the end of input
        strncpy(cmp, cmd, 4);
        cmp[4] = '\0';
        if(strcmp(cmp, "LIST") == 0){
            struct message{long type; char content[LENGTH+1];} message;
            message.type = LIST;
            sprintf(message.content, "%d", id);
            msgsnd(srv, &message, LENGTH, IPC_NOWAIT);
            msgrcv(q, &message, LENGTH, RESPONSE, 0);
        }
        else if(strcmp(cmp, "2ALL") == 0){
            struct message{long type; char content[LENGTH+1];} message; //+2??
            message.type = ALL;
            sprintf(message.content, "%d", id);
            strncpy(message.content+1, cmd+5, LENGTH-6);
            msgsnd(srv, &message, LENGTH, IPC_NOWAIT);
            printf("sent to all\n");
        }
        else if(strcmp(cmp, "2ONE") == 0){

        }
        else if(strcmp(cmp, "STOP") == 0){

        }
        else{
            printf("invalid command!\n");
        }
        free(cmd);
        free(cmp);
    }

    msgctl(q, IPC_RMID, NULL);
    return 0;
}