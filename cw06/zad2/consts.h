#ifndef CONSTS_H
#define CONSTS_H

#define SERVER_KEY 420
#define MAX_CLIENTS 8
#define LENGTH 1024
#define MAX_MESSAGES 10
#define MAX_PRIO sysconf(_SC_MQ_PRIO_MAX)

typedef enum{
    STOP = 1,
    LIST = 2,
    INIT = 3,
    RESPONSE = 4,
    ALL = 5,
    ONE = 6
} types;

#endif