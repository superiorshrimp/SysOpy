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

mqd_t q;
mqd_t clients[CLIENTS];
int active[CLIENTS];

void sigint_handle(int signum){
    exit(0);
}

void exit_handle(void){
    for(int i = 0; i<CLIENTS; i++){
        if(active[i] == 1){
            if(mq_close(clients[i]) == -1){printf("mq_close error\n");}
        }
    }
    if(mq_close(q) == -1){printf("mq_close error\n");}
    if(mq_unlink("/smq") == -1){printf("mq_unlink error\n");}
}

void init_handle(char* message){
    for(int i = 0; i<CLIENTS; i++){
        if(active[i] == 0){
            active[i] = 1;
            printf("%s\n", message);
            clients[i] = mq_open(message, O_RDWR);
            if(clients[i] == -1){printf("client mq_open error\n"); return;}
            char response[LENGTH];
            char id[LENGTH];
            sprintf(id, "%d", i);
            strcpy(response, id);
            mq_send(clients[i], response, LENGTH, MAX_PRIO - RSP);
            break;
        }
    }
    
}

void msg_handle(char* message){
    char name[2];
    name[1] = '\0';
    name[0] = message[0];
    int id = atoi(name);
    if(active[id] != 1){
        return;
    }
    printf("%s", message);
    char response[LENGTH];
    strcpy(response, "i got your message\n");
    mq_send(clients[id], response, LENGTH, MAX_PRIO - RSP);
}

int main(int argc, char* args[]){
    signal(SIGINT, sigint_handle);
    atexit(exit_handle);
    for(int i = 0; i<CLIENTS; i++){
        active[i] = 0;
    }

    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = LENGTH;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    q = mq_open("/smq", O_CREAT | O_RDWR, 0666, &attr);
    if(q == -1){printf("mq_open error s1\n"); return 1;}

    char message[LENGTH];
    unsigned int prio;
    while(1){
        mq_receive(q, message, LENGTH, &prio);
        switch(MAX_PRIO - prio){
            case STOP:
                printf("stop\n");
                break;
            case INIT:
                printf("init\n");
                init_handle(message);
                break;
            case MSG:
                printf("msg\n");
                msg_handle(message);
                break;
            default:
                printf("wrong type: %d\n", MAX_PRIO - prio);
                break;
        }
    }
    return 0;
}