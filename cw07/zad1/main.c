#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#define OVEN  5
#define TABLE 10

union semun {int val; struct semid_ds *buf; unsigned short *array;};

typedef struct Oven{
    int tab[OVEN];
    int index; 
}Oven;

typedef struct Table{
    int tab[TABLE];
    int index; 
}Table;

int cooks;
int deliv;
Oven* oven;
Table* table;
struct timeval start;

void cook(int sem){
    pid_t pid = getpid();
    srand((int)pid);
    struct timeval stop;
    for(int i = 0; i<8; i++){ //work day
        struct sembuf sb = {0,-1,0};
        int type = rand() % 10;
        float duration = 1000000 + rand() % 1000000;
        gettimeofday(&stop, NULL);
        printf("%ld, %d: przygotowuje pizze %d.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid, type);
        usleep(duration);
        int try = 1;
        int n = 0;
        int spot;
        while(try){
            sb.sem_num = 0;
            sb.sem_op = -1;
            if(semop(sem, &sb, 1) < 0){printf("semop error!\n"); exit(1);}
            if(oven->tab[oven->index] == -1){
                oven->tab[oven->index] = type;
                spot = oven->index;
                oven->index = (oven->index+1)%OVEN;
                try = 0;
                for(int j = 0; j<OVEN; j++){
                    if(oven->tab[j] != -1){n++;}
                }
            }
            sb.sem_op = 1;
            if(semop(sem, &sb, 1) < 0){printf("semop error!\n"); exit(1);}
        }
        gettimeofday(&stop, NULL);
        printf("%ld, %d: wstawilem pizze %d do pieca. liczba pizz w piecu: %d.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid, type, n);
        duration = 4000000 + rand() % 1000000;
        usleep(duration);
        try = 1;
        n = 0;
        int m = 0;
        struct sembuf sbh = {1,-1,0};
        while(try){
            sbh.sem_num = 1;
            sbh.sem_op = -1;
            if(semop(sem, &sbh, 1) < 0){printf("semop error!\n"); exit(1);}
            if(table->tab[table->index] == -1){
                sb.sem_op = -1;
                sb.sem_num = 0;
                if(semop(sem, &sb, 1) < 0){printf("semop error!\n"); exit(1);}
                oven->tab[spot] = -1;
                try = 0;
                for(int j = 0; j<OVEN; j++){
                    if(oven->tab[j] != -1){n++;}
                }
                sb.sem_op = 1;
                if(semop(sem, &sb, 1) < 0){printf("semop error!\n"); exit(1);}
            }
            table->tab[table->index] = type;
            table->index = (table->index+1)%TABLE;
            for(int j = 0; j<TABLE; j++){
                if(table->tab[j] != -1){m++;}
            }
            sbh.sem_op = 1;
            if(semop(sem, &sbh, 1) < 0){printf("semop error!\n"); exit(1);}
        }
        gettimeofday(&stop, NULL);
        printf("%ld, %d: wyjmuje pizze %d. w piecu zostalo %d pizz. na stole sa %d pizze.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid, type, n, m); 
    }
}

void deliver(int sem){
    pid_t pid = getpid();
    srand(pid);
    struct timeval stop;
    for(int i = 0; i<8; i++){
        struct sembuf sb = {1,-1,0};
        int try = 1;
        int type = -1;
        int m = 0;
        sb.sem_num = 1;
        while(try){
            sb.sem_op = -1;
            if(semop(sem, &sb, 1) < 0){printf("semop error!\n"); exit(1);}
            for(int j = 0; j<TABLE; j++){
                if(table->tab[j] != -1){
                    type = table->tab[j];
                    table->tab[j] = -1;
                    try = 0;
                    break;
                }
            }
            if(try == 0){
                for(int j = 0; j<TABLE; j++){
                    if(table->tab[j] != -1){m++;}
                }
            }
            sb.sem_op = 1;
            if(semop(sem, &sb, 1) < 0){printf("semop error!\n"); exit(1);}
        }
        gettimeofday(&stop, NULL);
        printf("%ld, %d: biore pizze %d. na stole zostalo %d pizz.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid, type, m);
        float duration = 4000000 + rand() % 1000000;
        usleep(duration);
        gettimeofday(&stop, NULL);
        printf("%ld, %d: dowiozlem pizze %d.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid, type);
        duration = 4000000 + rand() % 1000000;
        usleep(duration);
        gettimeofday(&stop, NULL);
        printf("%ld, %d: wrocilem.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid);
    }
}

int main(int argc, char* args[]){
    if(argc != 3){printf("pass number of cooks and delivery workers as an arguments!\n"); return 1;}
    cooks = atoi(args[1]);
    deliv = atoi(args[2]);
    printf("cooks: %d\ndelivery workers: %d\n", cooks, deliv);

    key_t skey = ftok("./main.c", 0);
    int sem = semget(skey, 2, IPC_CREAT | IPC_EXCL | 0666);
    if(sem == -1){
        sem = semget(skey, 2, 0666);
        if(sem != -1){
            semctl(sem, 0, IPC_RMID);
            sem = semget(skey, 2, IPC_CREAT | IPC_EXCL | 0666);
        }
    }
    union semun u;
    u.val = 1;
    if(semctl(sem, 0, SETVAL, u) < 0){printf("semctl error!\n"); return 1;}
    if(semctl(sem, 1, SETVAL, u) < 0){printf("semctl error!\n"); return 1;}

    key_t mkey1 = ftok("./main.c", 1);
    key_t mkey2 = ftok("./main.c", 2);
    int oven_mem = shmget(mkey1, sizeof(Oven), IPC_CREAT | IPC_EXCL | 0666);
    if(oven_mem == -1){
        oven_mem = shmget(mkey1, 0, 0666);
        if(oven_mem != -1){
            shmctl(oven_mem, IPC_RMID, NULL);
            oven_mem = shmget(mkey1, sizeof(Oven), IPC_CREAT | IPC_EXCL | 0666);
        }
    }
    int table_mem = shmget(mkey2, sizeof(Table), IPC_CREAT | 0666);
    if(table_mem == -1){
        table_mem = shmget(mkey2, 0, 0666);
        if(table_mem != -1){
            shmctl(table_mem, IPC_RMID, NULL);
            table_mem = shmget(mkey2, sizeof(Table), IPC_CREAT | IPC_EXCL | 0666);
        }
    }
    if(oven_mem == -1 || table_mem == -1){printf("shmget error!\n"); return 1;}

    oven = (Oven*)shmat(oven_mem, NULL, 0);
    oven->index = 0;
    for(int i = 0; i<OVEN; i++){oven->tab[i] = -1;}
    table = (Table*)shmat(table_mem, NULL, 0);
    table->index = 0;
    for(int i = 0; i<TABLE; i++){table->tab[i] = -1;}

    gettimeofday(&start, NULL);
    for(int i = 0; i<cooks; i++){
        if(fork() == 0){
            cook(sem);
            exit(0);
        }
    }
    for(int i = 0; i<deliv; i++){
        if(fork() == 0){
            deliver(sem);
            exit(0);
        }
    }
    while(wait(NULL) > 0){;}
    semctl(sem, 0, IPC_RMID);
    shmdt(oven);
    shmdt(table);
    shmctl(oven_mem, IPC_RMID, NULL);
    shmctl(table_mem, IPC_RMID, NULL);
    return 0;
}