#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include "consts.h"

mqd_t queue;
int active[MAX_CLIENTS];
mqd_t clients[MAX_CLIENTS];

void shutdown(int signum){
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(clients[i] != -1){
            if(mq_send(clients[i], "STOP", LENGTH, MAX_PRIO - STOP) == -1){
                mq_close(clients[i]);
                printf("problem with sending STOP message to client!\n");
            }
        }
    }

    if(mq_close(queue) == -1){printf("problem with closing queue!\n");}
    if(mq_unlink("/server") == -1){printf("problem with removing queue!\n");}
    FILE* f = fopen("./server_logs.txt", "a");
    fputs("\nserver closed! ", f);
    char* time = (char*)calloc(LENGTH, sizeof(char));
    sprintf(time, "%s", __TIME__);
    fputs(time, f);
    free(time);
    fclose(f);
    printf("server closed!\n");
    exit(0);
}

void stop(char* content){
    char* id = (char*)calloc(2, sizeof(char));
    id[0] = content[7];
    id[1] = '\0';
    int user = atoi(id);
    free(id);
    mq_close(clients[user]);
    clients[user] = (mqd_t)(-1);
    active[user] = -1;
}

void list(char* content){
    char* id = (char*)calloc(2, sizeof(char));
    id[0] = content[7];
    id[1] = '\0';
    int user = atoi(id);
    free(id);
    char* response = (char*)calloc(LENGTH, sizeof(char));
    strcat(response, "active users: ");
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(active[i] != -1){
            char* id = (char*)calloc(2, sizeof(char));
            sprintf(id, "%d", i);
            strcat(response, id);
            strcat(response, "; ");
            free(id);
        }
    }
    mq_send(clients[user], response, LENGTH, MAX_PRIO - INIT);
    free(response);
}

void init(char* content){
    char* id = (char*)calloc(2, sizeof(char));
    id[0] = content[7];
    id[1] = '\0';
    int user = atoi(id);
    clients[user] = mq_open(content, O_RDWR);
    if(clients[user] == -1){printf("problem with user INIT!\n");}
    char* response = (char*)calloc(LENGTH, sizeof(char));
    sprintf(response, "%c", content[7]);
    if(mq_send(clients[user], response, LENGTH, MAX_PRIO - RESPONSE) == -1){printf("problem with sending INIT message to client!\n");}
    active[user] = 1;
    printf("new client (ID: %d)\n", user);
    free(id);
    free(response);
}

void to_all(char* content){
    for(int i = 0; i<MAX_CLIENTS; i++){
        if(active[i] != -1){
            mq_send(clients[i], content, LENGTH, MAX_PRIO - ALL);
        }
    }
}

void to_one(char* content){
    printf("%s\n", content);
    char* id = (char*)calloc(2, sizeof(char));
    id[0] = content[1];
    id[1] = '\0';

    char* response = (char*)calloc(LENGTH, sizeof(char));
    strncpy(response, content, 1);
    strncpy(response+1, content+2, strlen(content)-1);

    mq_send(clients[atoi(id)], response, LENGTH, MAX_PRIO - ONE);
    free(response);
    free(id);
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
        fputs("from ", f);
        fputs(message, f);
        fputs(" at ", f);
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
    for(int i = 0; i<MAX_CLIENTS; i++){clients[i] = (mqd_t)(-1);}
    for(int i = 0; i<MAX_CLIENTS; i++){active[i] = -1;}
    signal(SIGINT, shutdown);
    FILE* f = fopen("./server_logs.txt", "w");

    struct mq_attr mq_attr;
    mq_attr.mq_flags = 0;
    mq_attr.mq_maxmsg = MAX_MESSAGES;
    mq_attr.mq_msgsize = LENGTH;
    mq_attr.mq_curmsgs = 0;

    queue = mq_open("/server", O_CREAT | O_RDONLY, 0666, &mq_attr);
    if(queue == -1){printf("problem with creating queue!\n"); exit(1);}
    fputs("server up and running! ", f);
    char* time = (char*)calloc(LENGTH, sizeof(char));
    sprintf(time, "%s", __TIME__);
    fputs(time, f);
    free(time);
    fclose(f);

    char message[LENGTH];
    unsigned int prio;
    while(1){
        mq_receive(queue, message, LENGTH, &prio);
        if(MAX_PRIO - prio == STOP){
            printf("user exiting\n");
            wr_log(message, STOP);
            stop(message);
        }
        else if(MAX_PRIO - prio == LIST){
            printf("received LIST request\n");
            wr_log(message, LIST);
            list(message);
        }
        else if(MAX_PRIO - prio == INIT){
            printf("received INIT request\n");
            wr_log(message, INIT);
            init(message);
        }
        else if(MAX_PRIO - prio == ALL){
            printf("sending message to all\n");
            wr_log(message, ALL);
            to_all(message);
        }
        else if(MAX_PRIO - prio == ONE){
            printf("sending DM\n");
            wr_log(message, ONE);
            to_one(message);
        }
    }

    return 0;
}