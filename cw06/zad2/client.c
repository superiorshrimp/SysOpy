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

mqd_t server;
mqd_t queue;
char* name;

void shutdown(int signum){
    if(signum != -1){
        mq_send(server, name, LENGTH, MAX_PRIO - STOP);
    }
    mq_close(server);
    mq_close(queue);
    mq_unlink(name);
    free(name);
    exit(0);
}

void refresh(void){
    char* message = (char*)calloc(LENGTH, sizeof(char));
    unsigned int prio;
    struct mq_attr mq_attr;
    mq_getattr(queue, &mq_attr);
    if(mq_attr.mq_curmsgs > 0){
        mq_receive(queue, message, LENGTH, &prio);
        if(MAX_PRIO - prio == STOP){
            printf("received STOP message!\n");
            shutdown(-1);
        }
        else if(MAX_PRIO - prio == ALL){
            printf("received 2ALL message!\n");
            printf("from: %c\n", message[0]);
            printf("content: %s\n", message+1);
        }
        else if(MAX_PRIO - prio == ONE){
            printf("received DM!\n");
            printf("from: %c\n", message[0]);
            printf("content: %s\n", message+1);
        }
    }
    
    free(message);
}

int main(int argc, char* args[]){
    if(argc != 2){printf("argc != 2\n"); return 1;}
    signal(SIGINT, shutdown);
    struct mq_attr mq_attr;
    mq_attr.mq_flags = 0;
    mq_attr.mq_maxmsg = MAX_MESSAGES;
    mq_attr.mq_msgsize = LENGTH;
    mq_attr.mq_curmsgs = 0;

    name = (char*)calloc(16, sizeof(char));
    strcpy(name, "/client");
    strcat(name, args[1]);
    printf("%s\n", name);
    queue = mq_open(name, O_CREAT | O_RDWR, 0666, &mq_attr);
    if(queue == -1){printf("problem with creating client queue!\n"); exit(1);}

    server = mq_open("/server", O_RDWR, 0666);
    if(server == -1){printf("problem with opening server queue!\n"); exit(1);}

    if(mq_send(server, name, LENGTH, MAX_PRIO - INIT) == -1){printf("problem with sending INIT request!\n"); exit(1);}
    
    char message[LENGTH];
    unsigned int prio;
    if(mq_receive(queue, message, LENGTH, &prio) == -1){printf("problem with receiving ID from server!\n"); exit(1);}
    printf("server accepted (ID: %s)\n", message);

    while(1){
        char* cmd = (char*)calloc(LENGTH, sizeof(char));
        char* cmp = (char*)calloc(5, sizeof(char));
        fgets(cmd, LENGTH, stdin);
        if(cmd[0] == '\n'){
            printf("refreshing...\n");
            refresh();
        }
        else{
            cmd[strcspn(cmd, "\n")] = 0; //delete newline at the end of input
            strncpy(cmp, cmd, 4);
            cmp[4] = '\0';
            if(strcmp(cmp, "LIST") == 0){
                mq_send(server, name, LENGTH, MAX_PRIO - LIST);
                mq_receive(queue, message, LENGTH, &prio);
                printf("%s\n", message);
            }
            else if(strcmp(cmp, "2ALL") == 0){
                char* msg = (char*)calloc(LENGTH, sizeof(char));
                msg[0] = name[7];
                strncpy(msg+1, cmd+5, LENGTH-6);
                mq_send(server, msg, LENGTH, MAX_PRIO - ALL);
                mq_receive(queue, message, LENGTH, &prio);
                free(msg);
            }
            else if(strcmp(cmp, "2ONE") == 0){ //2ONE7: do osoby 7
                char* msg = (char*)calloc(LENGTH, sizeof(char));
                msg[0] = name[7];
                msg[1] = cmd[4];
                strncpy(msg+2, cmd+6, LENGTH-7);
                mq_send(server, msg, LENGTH, MAX_PRIO - ONE);
                free(msg);
            }
            else if(strcmp(cmp, "STOP") == 0){
                mq_send(server, name, LENGTH, MAX_PRIO - STOP);
                shutdown(-1);
            }
            else{
                printf("invalid command!\n");
            }
        }
        free(cmd);
        free(cmp);
    }

    free(name);
    return 0;
}