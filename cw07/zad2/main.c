#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>

#define OVEN  5
#define TABLE 10

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

void cook(sem_t* oven_sem, sem_t* table_sem){
    pid_t pid = getpid();
    srand(pid);
    struct timeval stop;
    for(int i = 0; i<8; i++){
        int type = rand() % 10;
        float duration = 1000000 + rand() % 1000000;
        gettimeofday(&stop, NULL);
        printf("%ld, %d: przygotowuje pizze %d.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid, type);
        usleep(duration);
        int try = 1;
        int n = 0;
        int spot;
        while(try){
            sem_wait(oven_sem);
            if(oven->tab[oven->index] == -1){
                oven->tab[oven->index] = type;
                spot = oven->index;
                oven->index = (oven->index+1)%OVEN;
                try = 0;
                for(int j = 0; j<OVEN; j++){
                    if(oven->tab[j] != -1){n++;}
                }
            }
            sem_post(oven_sem);
        }
        gettimeofday(&stop, NULL);
        printf("%ld, %d: wstawilem pizze %d do pieca. liczba pizz w piecu: %d.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid, type, n);
        duration = 1000000 + rand() % 1000000;
        usleep(duration);
        try = 1;
        n = 0;
        int m = 0;
        while(try){
            sem_wait(table_sem);
            if(table->tab[table->index] == -1){
                sem_wait(oven_sem);
                oven->tab[spot] = -1;
                try = 0;
                for(int j = 0; j<OVEN; j++){
                    if(oven->tab[j] != -1){n++;}
                }
                sem_post(oven_sem);
                table->tab[table->index] = type;
                table->index = (table->index+1)%TABLE;
                for(int j = 0; j<TABLE; j++){
                    if(table->tab[j] != -1){m++;}
                }
            }
            sem_post(table_sem);
        }
        gettimeofday(&stop, NULL);
        printf("%ld, %d: wyjmuje pizze %d. w piecu zostalo %d pizz. na stole sa %d pizze.\n", 1000000*(stop.tv_sec-start.tv_sec)+stop.tv_usec-start.tv_usec, (int)pid, type, n, m);
    }
}

void deliver(sem_t* table_sem){
    pid_t pid = getpid();
    srand(pid);
    struct timeval stop;
    for(int i = 0; i<8; i++){
        int try = 1;
        int m = 0;
        int type = -1;
        while(try){
            sem_wait(table_sem);
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
            sem_post(table_sem);
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
    
    sem_t* oven_sem = sem_open("/oven", O_CREAT | O_EXCL, 0666, 1);
    if(oven_sem == SEM_FAILED){
        oven_sem = sem_open("/oven", 0);
        if(oven_sem != SEM_FAILED){
            sem_close(oven_sem);
            sem_unlink("/oven");
        }
        oven_sem = sem_open("/oven", O_CREAT | O_EXCL, 0666, 1);
    }
    sem_t* table_sem = sem_open("/table", O_CREAT | O_EXCL, 0666, 1);
    if(table_sem == SEM_FAILED){
        table_sem = sem_open("/table", 0);
        if(table_sem != SEM_FAILED){
            sem_close(table_sem);
            sem_unlink("/table");
        }
        table_sem = sem_open("/table", O_CREAT | O_EXCL, 0666, 1);
    }
    int oven_shm = shm_open("/oven", O_CREAT | O_EXCL | O_RDWR, 0666);
    if(oven_shm == -1){
        oven_shm = shm_open("/oven", 0, 0666);
        if(oven_shm != -1){
            shm_unlink("/oven");
        }
        oven_shm = shm_open("/oven", O_CREAT | O_EXCL | O_RDWR, 0666);
    }
    if(ftruncate(oven_shm, sizeof(Oven)) == -1){printf("ftruncate error!\n"); return 1;}
    int table_shm = shm_open("/table", O_CREAT | O_EXCL | O_RDWR, 0666);
    if(table_shm == -1){
        table_shm = shm_open("/table", 0, 0666);
        if(table_shm != -1){
            shm_unlink("/table");
        }
        table_shm = shm_open("/table", O_CREAT | O_EXCL | O_RDWR, 0666);
    }
    if(ftruncate(table_shm, sizeof(Table)) == -1){printf("ftruncate error!\n"); return 1;}
    
    oven = (Oven*)mmap(NULL, sizeof(Oven), PROT_READ | PROT_WRITE, MAP_SHARED, oven_shm, 0);
    for(int i = 0; i<OVEN; i++){oven->tab[i] = -1;}
    oven->index = 0;
    table = (Table*)mmap(NULL, sizeof(Table), PROT_READ | PROT_WRITE, MAP_SHARED, table_shm, 0);
    for(int i = 0; i<TABLE; i++){table->tab[i] = -1;}
    table->index = 0;

    gettimeofday(&start, NULL);
    for(int i = 0; i<cooks; i++){
        if(fork() == 0){
            cook(oven_sem, table_sem);
            exit(0);
        }
    }
    for(int i = 0; i<deliv; i++){
        if(fork() == 0){
            deliver(table_sem);
            exit(0);
        }
    }
    while(wait(NULL) > 0){;}

    if(sem_close(oven_sem) == -1){printf("sem_close error!\n"); return 1;}
    if(sem_unlink("/oven") == -1){printf("sem_unlink error!\n"); return 1;}
    if(sem_close(table_sem) == -1){printf("sem_close error!\n"); return 1;}
    if(sem_unlink("/table") == -1){printf("sem_unlink error!\n"); return 1;}
    shm_unlink("/oven");
    shm_unlink("/table");
    return 0;
}