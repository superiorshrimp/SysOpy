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

mqd_t sq;
mqd_t q;

void sigint_handle(int signum){
    exit(0);
}

void exit_handle(void){
    if(mq_close(sq) == -1){printf("mq_close error\n");}
    if(mq_close(q) == -1){printf("mq_close error\n");}
    if(mq_unlink("/cmq") == -1){printf("mq_unlink error\n");}
}

void refresh(void){
    struct mq_attr attr;
    mq_getattr(q, &attr);
    if(attr.mq_curmsgs > 0){
        char response[LENGTH];
        unsigned int prio;
        mq_receive(q, response, LENGTH, &prio);
        if(MAX_PRIO - prio == RSP){
            printf("%s\n", response);
            mq_send(sq, "0msg", LENGTH, MAX_PRIO - MSG);
            mq_receive(q, response, LENGTH, &prio);
            printf("%s\n", response);
        }
    }
}

int main(int argc, char* args[]){
    signal(SIGINT, sigint_handle);
    atexit(exit_handle);
    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_msgsize = LENGTH;
    attr.mq_maxmsg = 10;
    q = mq_open("/cmq", O_CREAT | O_RDWR, 0666, &attr);
    if(q == -1){printf("mq_open error c1\n"); return 1;}
    sq = mq_open("/smq", O_RDWR);
    if(sq == -1){printf("mq_open error c2\n"); return 1;}

    char message[LENGTH];
    strcpy(message, "/cmq");
    unsigned int prio = MAX_PRIO - INIT;
    printf("%s\n", message);
    mq_send(sq, message, LENGTH, prio);
    while(1){
        refresh();
    }

    return 0;
}