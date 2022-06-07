#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

int n = 5;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int common = 0;

typedef struct Data{
    int id;
    int seperate;
} Data;

void err(void){printf("%s\n", strerror(errno));}

void* f_thread(void* args){
    Data* data = (Data*)args;
    pthread_mutex_lock(&mutex);
    printf("%d: %d\n", data->id, common);
    printf("%d: %d\n", data->id, data->seperate);
    common++;
    data->seperate = data->seperate*data->seperate;
    printf("%d: %d\n", data->id, common);
    printf("%d: %d\n", data->id, data->seperate);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char* args[]){
    pthread_t* threads = (pthread_t*)calloc(n, sizeof(pthread_t));
    Data** arguments = (Data**)calloc(n, sizeof(Data*));

    for(int i = 0; i<n; i++){
        arguments[i] = (Data*)calloc(1, sizeof(Data));
        /*arguments[i]->seperate = (int*)calloc(1,sizeof(int));
        int val = 0;
        *arguments[i]->seperate = val;*/
        arguments[i]->seperate = i;
        arguments[i]->id = i;
        pthread_create(&threads[i], NULL, f_thread, (void*)arguments[i]);
    }
    for(int i = 0; i<n; i++){
        pthread_join(threads[i], NULL);
        
    }

    for(int i = 0; i<n; i++){
        printf("%d->%d\n", i, arguments[i]->seperate);
        free(arguments[i]);
    }
    free(arguments);
    free(threads);
    return 0;
}