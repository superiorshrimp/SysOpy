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
int sq;

void handle_exit(void){
    msgctl(q, IPC_RMID, NULL);
}

void ex(int signum){exit(0);}

int main(int argc, char* args[]){
    signal(SIGINT, ex);
    atexit(handle_exit);

    key_t key = ftok("./client.c", 0);
    q = msgget(key, IPC_CREAT | 0666);
    if(q == -1){
        q = msgget(key, 0666);
        if(q != -1){
            q = msgget(key, IPC_CREAT | 0666);
        } else{printf("msgget error\n"); return 1;}
    }

    sq = msgget(SERVER_KEY, 0666);

    struct message1{long type; char content[LENGTH];} message1;
    message1.type = INIT;
    sprintf(message1.content, "%d", q);
    msgsnd(sq, &message1, LENGTH, IPC_NOWAIT);
    struct response1{long type; char content[LENGTH];} response1;
    msgrcv(q, &response1, LENGTH, RSP, MSG_NOERROR);
    int id = atoi(response1.content);
    printf("id %d\n", id);

    struct message2{long type; char content[LENGTH];} message2;
    message2.type = MSG;
    char m[LENGTH];
    sprintf(m, "%dsample message\n", id);
    strcpy(message2.content, m);
    msgsnd(sq, &message2, LENGTH, IPC_NOWAIT);
    struct response2{long type; char content[LENGTH];} response2;
    msgrcv(q, &response2, LENGTH, RSP, MSG_NOERROR);
    printf("%s", response2.content);

    struct message3{long type; char content[LENGTH];} message3;
    message3.type = STOP;
    strcpy(message3.content, "bye\n");
    msgsnd(sq, &message3, LENGTH, IPC_NOWAIT);
    
    return 0;
}