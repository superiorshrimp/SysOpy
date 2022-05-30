#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#define ELVES     10
#define REINDEERS 9

typedef struct Data{
    int id;

    int* wake_up_santa;
    pthread_cond_t* santa_cond;
    pthread_mutex_t* santa_mutex;

    int* elves_waiting;
    pthread_mutex_t* elves_mutex;
    pthread_cond_t* elves_cond;
    int** which_elf;
    
    int* reindeers_waiting;
    pthread_mutex_t* reindeers_mutex;
    pthread_cond_t* reindeers_cond;
} Data;

void err(void){printf("%s\n", strerror(errno));}

void* santa_thread(void* args){
    Data* data = args;
    printf("Santa is sleeping...\n");
    while(1){
        pthread_mutex_lock(data->santa_mutex);
        pthread_cond_wait(data->santa_cond, data->santa_mutex);
        printf("Mikolaj: budze sie\n");
        *data->wake_up_santa = 0;
        pthread_mutex_unlock(data->santa_mutex);
        
        pthread_mutex_lock(data->reindeers_mutex);
        if(*data->reindeers_waiting == 9){
            *data->reindeers_waiting = 0;
            printf("Mikolaj: roznosi prezenty z reniferami\n");
            float duration = 2000000 + rand()%2000000;
            usleep(duration);
            printf("Mikolaj: wracam do spania...\n");
            pthread_cond_broadcast(data->reindeers_cond);
        }
        pthread_mutex_unlock(data->reindeers_mutex);
        
        pthread_mutex_lock(data->elves_mutex);
        if(*data->elves_waiting == 3){
            *data->elves_waiting = 0;
            int a = -1;
            int b = -1;
            int c = -1;
            for(int i = 0; i<ELVES; i++){
                if((*data->which_elf)[i] == 1){
                    if(a == -1){
                        a = i;
                    }
                    else if(b == -1){
                        b = i;
                    }
                    else{
                        c = i;
                    }
                }
                (*data->which_elf)[i] = 0;
            }
            printf("Mikolaj: rozwiazuje problemy elfow %d %d %d\n", a, b, c);
            float duration = 1000000 + rand()%1000000;
            usleep(duration);
            printf("Mikolaj: wracam do spania...\n");
            pthread_cond_broadcast(data->elves_cond);
        }
        pthread_mutex_unlock(data->elves_mutex);
    }
    pthread_exit(&data);
}

void* elf_thread(void* args){
    Data* data = args;
    printf("elf %d initialized\n", data->id);
    while(1){
        float duration = 2000000 + rand()%3000000;
        usleep(duration);
        pthread_mutex_lock(data->elves_mutex);
        if(*data->elves_waiting<2){
            (*data->which_elf)[data->id] = 1;
            (*data->elves_waiting)++;
            printf("Elf %d: %d elfow czeka na Mikolaja\n", data->id, *data->elves_waiting);
            pthread_cond_wait(data->elves_cond, data->elves_mutex);
            pthread_mutex_unlock(data->elves_mutex);
        }
        else if(*data->elves_waiting == 2){
            (*data->which_elf)[data->id] = 1;
            (*data->elves_waiting)++;
            printf("Elf %d: %d elfow czeka na Mikolaja\n", data->id, *data->elves_waiting);
            pthread_mutex_lock(data->santa_mutex);
            *data->wake_up_santa = 1;
            pthread_cond_broadcast(data->santa_cond);
            pthread_mutex_unlock(data->santa_mutex);
            pthread_cond_wait(data->elves_cond, data->elves_mutex);
            pthread_mutex_unlock(data->elves_mutex);
        }
        else{
            printf("Elf %d: czekam na powrot elfow od Mikolaja\n", data->id);
            pthread_mutex_unlock(data->elves_mutex);
            pthread_mutex_lock(data->elves_mutex);
            pthread_cond_wait(data->elves_cond, data->elves_mutex);
            pthread_mutex_unlock(data->elves_mutex);
        }
    }
    pthread_exit(&data);
}

void* reindeer_thread(void* args){
    Data* data = args;
    printf("reindeer %d initialized\n", data->id);
    while(1){
        float duration = 5000000 + rand()%5000000;
        usleep(duration);
        pthread_mutex_lock(data->reindeers_mutex);
        if(*data->reindeers_waiting<8){
            (*data->reindeers_waiting)++;
            printf("Renifer %d: %d reniferow czeka na inne renifery\n", data->id, *data->reindeers_waiting);
            pthread_cond_wait(data->reindeers_cond, data->reindeers_mutex);
            pthread_mutex_unlock(data->reindeers_mutex);
        }
        else if(*data->reindeers_waiting == 8){
            (*data->reindeers_waiting)++;
            printf("Renifer %d: %d reniferow czeka na inne renifery\n", data->id, *data->reindeers_waiting);
            pthread_mutex_lock(data->santa_mutex);
            *data->wake_up_santa = 1;
            pthread_cond_broadcast(data->santa_cond);
            pthread_mutex_unlock(data->santa_mutex);
            pthread_cond_wait(data->reindeers_cond, data->reindeers_mutex);
            pthread_mutex_unlock(data->reindeers_mutex);
        }
    }
    pthread_exit(&data);
}

int main(int argc, char* args[]){
    srand(time(NULL));
    pthread_t* threads = (pthread_t*)calloc(1+ELVES+REINDEERS, sizeof(pthread_t));
    Data** data = (Data**)calloc(1+ELVES+REINDEERS, sizeof(Data**));
    
    int wake_up_santa = 0;
    pthread_mutex_t smutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t scond = PTHREAD_COND_INITIALIZER;
    int elves_waiting = 0;
    pthread_mutex_t emutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t econd = PTHREAD_COND_INITIALIZER;
    int* which_elf = (int*)calloc(ELVES, sizeof(int));
    for(int i = 0; i<ELVES; i++){which_elf[i] = 0;}
    int reindeers_waiting = 0;
    pthread_mutex_t rmutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t rcond = PTHREAD_COND_INITIALIZER;
    //santa
    data[0] = (Data*)calloc(1,sizeof(Data));
    data[0]->wake_up_santa = &wake_up_santa;
    data[0]->santa_mutex = &smutex;
    data[0]->santa_cond = &scond;
    data[0]->elves_waiting = &elves_waiting;
    data[0]->elves_mutex = &emutex;
    data[0]->elves_cond = &econd;
    data[0]->which_elf = &which_elf;
    data[0]->reindeers_waiting = &reindeers_waiting;
    data[0]->reindeers_mutex = &rmutex;
    data[0]->reindeers_cond = &rcond;
    //elves
    for(int i = 1; i<1+ELVES; i++){
        data[i] = (Data*)calloc(1,sizeof(Data));
        data[i]->id = i-1;
        data[i]->wake_up_santa = &wake_up_santa;
        data[i]->santa_mutex = &smutex;
        data[i]->santa_cond = &scond;
        data[i]->elves_waiting = &elves_waiting;
        data[i]->elves_mutex = &emutex;
        data[i]->elves_cond = &econd;
        data[i]->which_elf = &which_elf;
    }
    //reindeers
    for(int i = 1+ELVES; i<1+ELVES+REINDEERS; i++){
        data[i] = (Data*)calloc(1,sizeof(Data));
        data[i]->id = i-1-ELVES;
        data[i]->wake_up_santa = &wake_up_santa;
        data[i]->santa_mutex = &smutex;
        data[i]->santa_cond = &scond;
        data[i]->reindeers_waiting = &reindeers_waiting;
        data[i]->reindeers_mutex = &rmutex;
        data[i]->reindeers_cond = &rcond;
    }
    //santa
    pthread_create(&threads[0], NULL, santa_thread, data[0]);
    //elves
    for(int i = 1; i<1+ELVES; i++){pthread_create(&threads[i], NULL, elf_thread, data[i]);}
    //reindeers
    for(int i = 1+ELVES; i<1+ELVES+REINDEERS; i++){pthread_create(&threads[i], NULL, reindeer_thread, data[i]);}

    for(int i = 0; i<1+ELVES+REINDEERS; i++){pthread_join(threads[i], NULL);}
    for(int i = 0; i<1+ELVES+REINDEERS; i++){free(data[i]);}
    free(which_elf);
    free(data);
    free(threads);
    return 0;
}