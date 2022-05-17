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
#include <sys/time.h>
#include <ctype.h>

typedef struct image{
    int width;
    int height;
    int** tab;
} image;

void err(void){printf("%s\n", strerror(errno));}

void read_image(FILE* src, image* img){
    int i = 0;
    int step = 1;
    char c = fgetc(src);
    char* width = (char*)calloc(16, sizeof(char));
    char* height = (char*)calloc(16, sizeof(char));
    char* letter = (char*)calloc(2, sizeof(char));
    char* val = (char*)calloc(4, sizeof(char));
    val[3] = '\0';
    letter[1] = '\0';
    char last = '\0';
    int col = 0;
    while(c != EOF){
        if(i == 1){
            letter[0] = c;
            if(step == 0 && isspace(c)){step = 1;}
            if(step == 1 && !isspace(c)){step = 2;}
            if(step == 0){
                strcat(width, letter);
            }
            if(step == 2 && c != '\n'){
                strcat(height, letter);
            }
        }
        else if(i == 3){
            img->width = atoi(width);
            img->height = atoi(height);
            img->tab = (int**)calloc(img->height, sizeof(int*));
            for(int row = 0; row<img->height; row++){
                img->tab[row] = (int*)calloc(img->width, sizeof(int));
            }
        }
        if(i >= 3){
            int row = i-3;
            if(isspace(c) && !isspace(last))
        }
        if(c == '\n'){i++; col = 0;}

        last = c;
        c = fgetc(src);
    }
    free(width);
    free(height);
    free(letter);
}

void numbers(int n, image* img, FILE* res){

}

void blocks(int n, image* img, FILE* res){

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

    FILE* res = fopen(results_name, "w");
    if(res == NULL){printf("fopen error!\n"); return 1;}
    if(mode == 0){
        numbers(n, img, res);
    }
    else if(mode == 1){
        blocks(n, img, res);
    }
    else{printf("wrong mode val!\n");}
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