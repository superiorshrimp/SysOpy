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
int clients[MAX_CLIENTS];

void server_shutdown(int signum){
    printf("closing server...\n");
    msgctl(q, IPC_RMID, NULL);
    exit(0); //to call atexit
}
void client_shutdown(void){
    msgctl(q, IPC_RMID, NULL);
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i] != -1){
            msgctl(clients[i], IPC_RMID, NULL);
            struct message{long type; char content[LENGTH+1];} message;
            message.type = STOP;
            msgsnd(clients[i], &message, LENGTH, IPC_NOWAIT);
        }
    }
}

void list(int from){
    char* response = (char*)calloc(LENGTH+1, sizeof(char));
    strcat(response, "active users: ");
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i] != -1){
            char* id = (char*)calloc(2, sizeof(char));
            sprintf(id, "%d", i);
            strcat(response, id);
            strcat(response, "; ");
            free(id);
        }
    }
    printf("%s\n", response);
    struct message{long type; char content[LENGTH+1];} message;
    message.type = RESPONSE;
    strcat(message.content, response);
    msgsnd(clients[from], &message, LENGTH, IPC_NOWAIT);
    free(response);
}

void init(char* content){
    char* ptr;
    int id = (int)strtol(content, &ptr, 10);
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i] == id){
            printf("already initialized!\n");
            struct message{long type; char content[LENGTH+1];} message;
            message.type = RESPONSE;
            sprintf(message.content, "%d", -1);
            msgsnd(clients[i], &message, LENGTH, 0);
            return;
        }
    }
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i] == -1){
            clients[i] = (int)strtol(content, &ptr, 10);
            struct message{long type; char content[LENGTH+1];} message;
            message.type = RESPONSE;
            sprintf(message.content, "%d", i);
            msgsnd(clients[i], &message, LENGTH, 0);
            return;
        }
    }  
}

void to_all(int from, char* msg){
    char* start = (char*)calloc(4, sizeof(char));
    sprintf(start, "%d: ", from);
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i] != -1 && i != from){
            struct message{long type; char content[LENGTH+1];} message;
            message.type = ALL;
            strcat(message.content, start);
            strcat(message.content, msg);
            msgsnd(clients[i], &message, LENGTH, IPC_NOWAIT);
        }
    }
    free(start);
}

int main(int argc, char* args[]){
    signal(SIGINT, server_shutdown);
    atexit(client_shutdown);
    for(int i = 0; i<MAX_CLIENTS; i++){clients[i] = -1;}
    int q = msgget(SERVER_KEY, 0666|IPC_CREAT);
    if(q == -1){
        printf("server msgget error!\n");
        return 1;
    }
    printf("Server ready with id: %d\n", q);
    while(1){
        struct message{long type; char content[LENGTH+1];} message;
        msgrcv(q, &message, sizeof(message.content), 0, MSG_NOERROR); //for now 0
        if(message.type == STOP){
            printf("Exiting...\n"); //FIXME: znaczenie
            server_shutdown(-1);
        }
        else if(message.type == LIST){
            printf("received LIST request\n");
            list(atoi(message.content));
        }
        else if(message.type == INIT){
            printf("received INIT request\n\n");
            init(message.content);
        }
        else if(message.type == ALL){
            printf("sending message to all\n");
            char* id = (char*)calloc(2, sizeof(char));
            char* msg = (char*)calloc(strlen(message.content)+1, sizeof(char));
            strncpy(id, message.content, 1);
            strncpy(msg, message.content+1, strlen(message.content)-1);
            msg[strlen(message.content)] = '\0';
            id[1] = '\0';
            printf("from: %s\n", id);
            printf("content: %s\n", msg);
            to_all(atoi(id), msg);
            free(id);
            free(msg);
        }
        else if(message.type == ONE){
            printf("sending DM\n");
        }
    }

    server_shutdown(-1);
    return 0;
}