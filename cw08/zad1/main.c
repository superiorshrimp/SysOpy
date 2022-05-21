#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

typedef struct image{
    int width;
    int height;
    int** tab;
} image;

typedef struct thread_data{
    int* a;
    int* b;
    image* img;
    double* time;
    int** updated;
} thread_data;

int colors;

void err(void){printf("%s\n", strerror(errno));}

void read_image(FILE* src, image* img){
    char* type = (char*)calloc(3, sizeof(char));
    fread(type, sizeof(char), 2, src);
	type[2] = '\0';
    if(strcmp(type, "P2") != 0){printf("wrong file header!\n"); exit(1);}
    free(type);
    int width;
    int height;
    fscanf(src, "%d %d\n", &width, &height);
    img->width = width;
    img->height = height;
    img->tab = (int**)calloc(img->height, sizeof(int*));
    for(int i = 0; i<height; i++){
        img->tab[i] = (int*)calloc(img->width, sizeof(int));
    }
    fscanf(src, "%d\n", &colors);
    int val;
    for(int i = 0; i<img->height; i++){
        for(int j = 0; j<img->width; j++){
            fscanf(src, "%u", &val);
            img->tab[i][j] = val;
        }
    }
}

void save_image(FILE* f, image* img){
    fputs("P2\n", f);
    char* width = (char*)calloc(8,sizeof(char));
    char* height = (char*)calloc(8,sizeof(char));
    sprintf(width, "%d", img->width);
    sprintf(height, "%d", img->height);
    fputs(width, f);
    fputs(" ", f);
    fputs(height, f);
    free(width);
    free(height);
    fputs("\n", f);
    char* colors_char = (char*)calloc(4,sizeof(char));
    sprintf(colors_char, "%d", colors);
    fputs(colors_char, f);
    fputs("\n", f);
    free(colors_char);
    int k = 1;
    char* val = (char*)calloc(4, sizeof(char));
    for(int i = 0; i<img->height; i++){
        for(int j = 0; j<img->width; j++){
            sprintf(val, "%d", img->tab[i][j]);
            fputs(val, f);
            free(val);
            val = (char*)calloc(4, sizeof(char));
            if(k%15 == 0){
                fputs("\n", f);
            }
            else{ 
                fputs(" ", f);
            }
            k++;
        }
    }
    free(val);
}

void* numbers_thread(void* arg){
    thread_data* data = arg;
    int a = *data->a;
    int b = *data->b;
    image* img = data->img;
    int** updated = data->updated;
    clock_t start = clock();
    for(int i = 0; i<img->height; i++){
        for(int j = 0; j<img->width; j++){
            if(a<=img->tab[i][j] && b>img->tab[i][j] && updated[i][j] == 0){
                updated[i][j] = 1;
                img->tab[i][j] = colors - img->tab[i][j];
            }
        }
    }
    data->time = (double*)calloc(1,sizeof(double));
    double t = (double)(clock() - start)/CLOCKS_PER_SEC;
    *data->time = t;
    pthread_exit(&data);
}

void numbers(int n, image* img){
    int* pixels = (int*)calloc(colors+1, sizeof(int));
    for(int i = 0; i<colors+1; i++){pixels[i] = 0;}
    for(int i = 0; i<img->height; i++){
        for(int j = 0; j<img->width; j++){
            pixels[img->tab[i][j]]++;
        }
    }
    int** updated = (int**)calloc(img->height, sizeof(int*));
    for(int i = 0; i<img->height; i++){
        updated[i] = (int*)calloc(img->width, sizeof(int));
        for(int j = 0; j<img->width; j++){
            updated[i][j] = 0;
        }
    }
    pthread_t* threads = (pthread_t*)calloc(n,sizeof(pthread_t));
    thread_data** args = (thread_data**)calloc(n,sizeof(thread_data*));
    int val = (int)(img->width*img->height/n);
    int a = 0;
    int b = 0;
    int k = 0;
    int last;
    int count;
    while(k < n){
        count = 0;
        while(count <= k*val){
            last = count;
            b++;
            if(b>=colors){break; k = n;}
            count += pixels[b];
        }
        if(abs(count - k*val) > abs(last - k*val)){
            b = last;
        }
        args[k] = (thread_data*)calloc(1,sizeof(thread_data));
        args[k]->a = (int*)calloc(1,sizeof(int));
        *args[k]->a = a;
        args[k]->b = (int*)calloc(1,sizeof(int));
        *args[k]->b = b;
        args[k]->img = img;
        args[k]->updated = updated;
        pthread_create(&threads[k], NULL, numbers_thread, args[k]);
        a = b;
        k++;
    }
    for(int k = 0; k<n; k++){
        pthread_join(threads[k], NULL);
        printf("%lf\n", *args[k]->time);
    }
    for(int i = 0; i<img->height; i++){
        free(updated[i]);
    }
    free(updated);
    free(pixels);
    free(threads);
    for(int i = 0; i<n; i++){
        free(args[i]->a);
        free(args[i]->b);
        free(args[i]->time);
        free(args[i]);
    }
    free(args);
}

void* blocks_thread(void* arg){
    thread_data* data = arg;
    int a = *data->a;
    int b = *data->b;
    image* img = data->img;
    clock_t start = clock();
    for(int x = a; x<=b; x++){
        for(int y = 0; y<img->height; y++){
            img->tab[y][x] = colors - img->tab[y][x];
        }
    }
    data->time = (double*)calloc(1,sizeof(double));
    double t = (double)(clock() - start)/CLOCKS_PER_SEC;
    *data->time = t;
    pthread_exit(&data);
}

void blocks(int n, image* img){
    int part = (int)(img->width/n);
    pthread_t* threads = (pthread_t*)calloc(n,sizeof(pthread_t));
    thread_data** args = (thread_data**)calloc(n,sizeof(thread_data*));
    for(int k = 0; k<n-1; k++){
        args[k] = (thread_data*)calloc(1,sizeof(thread_data));
        int a = k*part;
        int b = (k+1)*part-1;
        args[k]->a = (int*)calloc(1,sizeof(int));
        *args[k]->a = a;
        args[k]->b = (int*)calloc(1,sizeof(int));
        *args[k]->b = b;
        args[k]->img = img;
        pthread_create(&threads[k], NULL, blocks_thread, args[k]);
    }
    int a = (n-1)*part;
    int b = img->width-1;
    args[n-1] = (thread_data*)calloc(1,sizeof(thread_data));
    args[n-1]->a = (int*)calloc(1,sizeof(int));
    *args[n-1]->a = a;
    args[n-1]->b = (int*)calloc(1,sizeof(int));
    *args[n-1]->b = b;
    args[n-1]->img = img;
    pthread_create(&threads[n-1], NULL, blocks_thread, (void*)(args[n-1]));
    for(int k = 0; k<n; k++){
        pthread_join(threads[k], NULL);
        printf("%lf\n", *args[k]->time);
    }
    free(threads);
    for(int i = 0; i<n; i++){
        free(args[i]->a);
        free(args[i]->b);
        free(args[i]->time);
        free(args[i]);
    }
    free(args);
}

int main(int argc, char* args[]){
    if(argc != 5){printf("4 args required!\n"); return 1;}
    int n = atoi(args[1]);
    int mode = atoi(args[2]);
    char* source_name = (char*)calloc(128, sizeof(char));
    strcat(source_name, args[3]);
    char* results_name = (char*)calloc(128, sizeof(char));
    strcat(results_name, args[4]);

    FILE* src = fopen(source_name, "r");
    if(src == NULL){printf("fopen error!\n"); return 1;}
    image* img = (image*)calloc(1,sizeof(image));
    read_image(src, img);
    if(fclose(src) != 0){printf("fclose error!\n"); return 1;}

    if(mode == 0){
        numbers(n, img);
    }
    else if(mode == 1){
        blocks(n, img);
    }
    else{printf("wrong mode val!\n");}

    /*for(int i = 0; i<img->height; i++){
        for(int j = 0; j<img->width; j++){
            printf("%d", img->tab[i][j]);
            if(img->tab[i][j] >= 10){
                printf(" ");
            }
            else{
                printf("  ");
            }
        }
        printf("\n");
    }*/

    FILE* res = fopen(results_name, "w");
    if(res == NULL){printf("fopen error!\n"); return 1;}
    save_image(res, img);
    if(fclose(res) != 0){printf("fclose error!\n"); return 1;}
    free(source_name);
    free(results_name);
    for(int row = 0; row<img->height; row++){
        free(img->tab[row]);
    }
    free(img->tab);
    free(img);
    return 0;
}