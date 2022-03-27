#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>

int is_txt(char *name){
    int len = strlen(name);
    if(len>4){
        char* ext = (char*)calloc(5, sizeof(char));
        strcat(ext, &name[len-4]);
        if(strcmp(ext, ".txt") == 0){ 
            return 1;
        }
    }
    return 0;
}

int is_in(char* target, char* path){
    FILE* f = fopen(path, "r");
    int i = 0;
    int len = strlen(target);
    char c = fgetc(f);
    while(c != EOF){
        if(c == target[i]){
            i++;
            if(i == len){
                fclose(f);
                return 1;
            }
        }
        else{
            i = 0;
        }
        c = fgetc(f);
    }
    fclose(f);
    return 0;
}

void check_file(struct dirent* dir, char* path, char* target){
    struct stat* file = malloc(sizeof(struct stat));
    stat(path, file);
    if(is_in(target, path) == 1){
        printf("process pid: %d\npath: %s\nfile name: %s\n\n", (int)getpid(), path, dir->d_name);
    }
    free(file);
}

void go_deeper(char* name, char* target, int depth){
    if(depth == 0){return;}
    DIR* root = opendir(name);
    struct dirent* dir = readdir(root);
    while(dir != NULL){
        char* path = (char*)calloc(257, sizeof(char));
        strcat(path, name);
        if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
            strcat(path, "/");
            strcat(path, dir->d_name);
            if(dir->d_type == 4){ //directory
                pid_t child_pid = fork();
                if(child_pid == 0){ //child
                    go_deeper(path, target, depth - 1);
                    exit(0);
                }
            }
            else if(dir->d_type == 8 && is_txt(dir->d_name) == 1){ //txt file
                check_file(dir, path, target);
            }
        }
        free(path);
        dir = readdir(root);
    }
    closedir(root);
    while(wait(NULL) > 0);
}

int main(int argc, char* args[]){
    if(argc != 4){return 1;}
    char* path = (char*)calloc(257, sizeof(char));
    strcat(path, args[1]);
    char* target = (char*)calloc(257, sizeof(char));
    strcat(target, args[2]);
    int depth = atoi(args[3]);
    go_deeper(path, target, depth);
    free(path);
    free(target);
    return 0;
}