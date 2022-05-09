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
    FILE* f = fopen("./server_logs.txt", "a");
    fputs("\nserver closed! ", f);
    char* time = (char*)calloc(LENGTH, sizeof(char));
    sprintf(time, "%s", __TIME__);
    fputs(time, f);
    free(time);
    fclose(f);
    exit(0); //to call atexit
}

void client_shutdown(void){
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i] != -1){
            struct message{long type; char content[LENGTH+1];} message;
            message.type = STOP;
            msgsnd(clients[i], &message, LENGTH, IPC_NOWAIT);
            //msgctl(clients[i], IPC_RMID, NULL);
        }
    }
    msgctl(q, IPC_RMID, NULL);
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
    strcpy(message.content, response);
    message.content[strlen(response)] = '\0';
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
    char* response = (char*)calloc(LENGTH+1, sizeof(char));
    sprintf(start, "%d: ", from);
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i] != -1 && i != from){
            struct message{long type; char content[LENGTH+1];} message;
            message.type = ALL;
            strcat(response, start);
            strcat(response, msg);
            strcpy(message.content, response);
            message.content[strlen(response)] = '\0';
            msgsnd(clients[i], &message, LENGTH, 0);
        }
    }
    free(start);
    free(response);
}

void to_one(char* from, int to, char* msg){
    if(clients[to] == -1){
        printf("user not on server!\n");
    }
    else{
        char* response = (char*)calloc(LENGTH+1, sizeof(char));
        struct message{long type; char content[LENGTH+1];} message;
        message.type = ONE;
        strcat(response, from);
        strcat(response, msg);
        strcpy(message.content, response);
        message.content[strlen(response)] = '\0';
        msgsnd(clients[to], &message, LENGTH, IPC_NOWAIT);
        free(response);
    }
}

void wr_log(char* message, long type){
    FILE* f = fopen("./server_logs.txt", "a");
    fputs("\n", f);
    fputs("received ", f);
    char* from = (char*)calloc(LENGTH, sizeof(char));
    char* to = (char*)calloc(LENGTH, sizeof(char));
    char* time = (char*)calloc(LENGTH, sizeof(char));
    sprintf(time, __TIME__);
    if(type == STOP){
        fputs("STOP ", f);
        fputs("from ", f);
        fputs(message, f);
        fputs(" at ", f);
    }
    else if(type == LIST){
        fputs("LIST ", f);
        fputs("from ", f);
        fputs(message, f);
        fputs(" at ", f);
    }
    else if(type == INIT){
        fputs("INIT ", f);
        fputs("at ", f);
    }
    else if(type == RESPONSE){
        fputs("RESPONSE ", f);
        fputs("from ", f);
        fputs(message, f);
        fputs(" at ", f);
    }
    else if(type == ALL){
        fputs("2ALL ", f);
        fputs("from ", f);
        sprintf(from, "%c", message[0]);
        fputs(from, f);
        fputs(" at ", f);
    }
    else if(type == ONE){
        fputs("2ONE ", f);
        fputs("from ", f);
        sprintf(from, "%c", message[0]);
        fputs(from, f);
        fputs(" to ", f);
        sprintf(to, "%c", message[1]);
        fputs(to, f);
        fputs(" at ", f);
    }
    fputs(time, f);
    free(from);
    free(time);
    free(to);
    fclose(f);
}

int main(int argc, char* args[]){
    signal(SIGINT, server_shutdown);
    atexit(client_shutdown);
    for(int i = 0; i<MAX_CLIENTS; i++){clients[i] = -1;}
    
    FILE* f = fopen("./server_logs.txt", "w");
    int q = msgget(SERVER_KEY, 0666|IPC_CREAT);
    if(q == -1){
        printf("server msgget error!\n");
        return 1;
    }
    fputs("server up and running! ", f);
    char* time = (char*)calloc(LENGTH, sizeof(char));
    sprintf(time, "%s", __TIME__);
    fputs(time, f);
    free(time);
    fclose(f);

    printf("Server ready with id: %d\n", q);
    while(1){
        struct message{long type; char content[LENGTH+1];} message;
        msgrcv(q, &message, LENGTH, -10, MSG_NOERROR);
        if(message.type == STOP){
            printf("user exiting\n");
            wr_log(message.content, message.type);
            msgctl(clients[atoi(message.content)], IPC_RMID, NULL);
            clients[atoi(message.content)] = -1;
        }
        else if(message.type == LIST){
            printf("received LIST request\n");
            wr_log(message.content, message.type);
            list(atoi(message.content));
        }
        else if(message.type == INIT){
            printf("received INIT request\n\n");
            wr_log(message.content, message.type);
            init(message.content);
        }
        else if(message.type == ALL){
            printf("sending message to all\n");
            wr_log(message.content, message.type);
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
            wr_log(message.content, message.type);
            char* id = (char*)calloc(2, sizeof(char));
            char* to = (char*)calloc(2, sizeof(char));
            char* msg = (char*)calloc(strlen(message.content)+1, sizeof(char));
            sprintf(id, "%c", message.content[0]);
            sprintf(to, "%c", message.content[1]);
            strncpy(msg, message.content+2, strlen(message.content)-2);
            msg[strlen(message.content)] = '\0'; //takie indeksowanie??
            to_one(id, atoi(to), msg);
            free(id);
            free(to);
            free(msg);
        }
    }

    server_shutdown(-1);
    return 0;
}