
#ifndef CONSTS_H
#define CONSTS_H

#define LENGTH 1024
#define SERVER_KEY 420
#define CLIENTS 3
#define MAX_PRIO 10

typedef enum{
    STOP = 1,
    INIT = 2,
    MSG = 3,
    RSP = 4
} type;

#endif