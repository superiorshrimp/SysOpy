#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <wait.h>

int sem;
int mem;
int n = 3;

typedef struct Data{
    int a;
} Data;

Data* data;

union semun{int val; struct semid_ds *buf; unsigned short *array;};

void f(int s){
    pid_t pid = getpid();
    for(int i = 0; i<3; i++){
        struct sembuf sb;
        sb.sem_op = -1;
        sb.sem_num = 0;
        sb.sem_flg = 0;
        if(semop(s, &sb, 1) < 0){printf("semop error\n"); exit(1);}
        printf("%d %d: %d\n", pid, i, data->a);
        data->a = data->a + 1;
        printf("%d %d: %d\n", pid, i, data->a);
        sb.sem_op = 1;
        if(semop(s, &sb, 1) < 0){printf("semop error\n"); exit(1);}
    }
}

int main(int argc, char* args[]){
    key_t skey = ftok("./main.c", 0);
    sem = semget(skey, 1, IPC_CREAT | IPC_EXCL | 0666);
    if(sem == -1){
        sem = semget(skey, 1, 0666);
        if(sem != -1){
            semctl(sem, 0, IPC_RMID);
            sem = semget(skey, 1, IPC_CREAT | IPC_EXCL | 0666);
        }
    }

    union semun u;
    u.val = 1;
    if(semctl(sem, 0, SETVAL, u) == -1){printf("semctl error!\n"); return 1;}

    key_t mkey = ftok("./main.c", 1);
    mem = shmget(mkey, sizeof(Data), IPC_CREAT | IPC_EXCL | 0666);
    if(mem == -1){
        mem = shmget(mkey, 0, 0666);
        if(mem != -1){
            shmctl(mem, IPC_RMID, NULL);
            mem = shmget(mkey, sizeof(Data), IPC_CREAT | IPC_EXCL | 0666);
        }
    }

    if(mem == -1){printf("shm error\n"); return 1;}
    data = (Data*)shmat(mem, NULL, 0);
    data->a = 0;

    for(int i = 0; i<n; i++){
        if(fork() == 0){
            f(sem);
            exit(0);
        }
    }
    while(wait(NULL) > 0){;}

    semctl(sem, 0, IPC_RMID);
    shmdt(data);
    shmctl(mem, IPC_RMID, NULL);
    return 0;
}