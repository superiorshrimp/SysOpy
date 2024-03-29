#ifndef CONSTS_H
#define CONSTS_H

#define SERVER_KEY 420
#define MAX_CLIENTS 8
#define LENGTH 1024

typedef enum{
    STOP = 1, //needs to be 1, because 0 causes msgsnd to return -1
    LIST = 2,
    INIT = 3,
    RESPONSE = 4,
    ALL = 5,
    ONE = 6
} types;

#endif