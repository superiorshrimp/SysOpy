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

sem_t* sem;
int mem;
int n = 3;

typedef struct Data{
    int a;
} Data;

Data* data;

void f(sem_t* s){
    pid_t pid = getpid();
    for(int i = 0; i<3; i++){
        sem_wait(s);
        printf("%d %d: %d\n", pid, i, data->a);
        data->a = data->a + 1;
        printf("%d %d: %d\n", pid, i, data->a);
        sem_post(s);
    }
}

int main(int argc, char* args[]){
    sem = sem_open("/exsem", O_CREAT | O_EXCL, 0666, 1);
    if(sem == SEM_FAILED){
        sem = sem_open("/exsem", 0);
        if(sem != SEM_FAILED){
            sem_close(sem);
            sem_unlink("/exsem");
        }
        sem = sem_open("/exsem", O_CREAT | O_EXCL, 0666, 1);
    }
    mem = shm_open("/exmem", O_CREAT | O_EXCL | O_RDWR, 0666);
    if(mem == -1){
        mem = shm_open("/exmem", 0, 0666);
        if(mem != -1){
            shm_unlink("/exmem");
        }
        mem = shm_open("/exmem", O_CREAT | O_EXCL | O_RDWR, 0666);
    }
    if(ftruncate(mem, sizeof(Data)) == -1){printf("ftruncate error!\n"); return 1;}
    data = (Data*)mmap(NULL, sizeof(Data), PROT_READ | PROT_WRITE, MAP_SHARED, mem, 0);
    for(int i = 0; i<n; i++){
        if(fork() == 0){
            f(sem);
            exit(0);
        }    
    }
    while(wait(NULL) > 0){;}

    if(sem_close(sem) == -1){printf("sem_close error\n"); return 1;}
    if(sem_unlink("/exsem") == -1){printf("sem_unlink error\n"); return 1;}
    if(munmap(data, sizeof(data)) == -1){printf("munmap error\n"); return 1;}
    if(shm_unlink("/exmem") == -1){printf("shm_unlink error\n"); return 1;}
    return 0;
}