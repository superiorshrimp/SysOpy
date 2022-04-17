#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>

void create_file(char* path){
    FILE* f = fopen(path, "w");
    for(int row = 0; row<=10; row++){
        fputc('\n', f);
    }
    fclose(f);
}

void wr(char* content, char* path, int N){
    int row = content[0] - '0';
    FILE* f = fopen(path, "r");
    char* before = (char*)calloc(100*N, sizeof(char));
    char* after = (char*)calloc(100*N, sizeof(char));
    int flag = 0;
    int counter = 0;
    int l = 0;
    int p = 0;
    char c = fgetc(f);
    while(c != EOF){
        if(c == '\n'){
            if(row == counter){
                flag = 1;
            }
            counter++;
        }
        if(flag == 0){
            before[l] = c;
            l++;
        }
        else{
            after[p] = c;
            p++;
        }
        c = fgetc(f);
    }
    fclose(f);

    f = fopen(path, "w");
    fputs(before, f);
    char* slice = (char*)calloc(N+1, sizeof(char));
    for(int i = 0; i<N; i++){
        slice[i] = content[1+i];
    }
    fputs(slice, f);
    fputs(after, f);

    free(slice);
    free(before);
    free(after);
    fclose(f);
}

int main(int argc, char* args[]){
    if(argc != 3){return 1;}
    char* path = args[0]; //fifo
    char* file = args[1]; //results
    int N = atoi(args[2]);
    create_file(file);

    int fd = open(path, O_RDONLY);
    if(fd == -1){return 1;}

    char* buffer = calloc(N+2, sizeof(char));
    
    int run = 1;
    int last = 0;
    
    while(run == 1){
        usleep(100000);
        read(fd, buffer, N+1);
        if(buffer[0] == '\0'){
            last++;
            if(last == 21){run = 0;}
        }
        else{
            last = 0;
            wr(buffer, file, N);
        }
        free(buffer);
        buffer = calloc(N+2, sizeof(char));
    }
    free(buffer);
    close(fd);

    return 0;
}