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

int counter[] = {0, 1, 0, 0, 0, 0, 0};

void get_info(struct dirent* dir, char* path){
    FILE* f = fopen("./results.txt", "a");
    fputs(dir->d_name, f);
    printf("%s\n", dir->d_name);
    fputc('\n', f);
    fputs("path: ", f);
    fputs(path, f);
    fputc('\n', f);
    printf("%s\n", path);
    
    struct stat* file = malloc(sizeof(struct stat));
    stat(path, file);
    
    char* links = (char*)calloc(33, sizeof(char));
    fputs("links: ", f);
    sprintf(links, "%ld", file->st_nlink);
    fputs(links, f);
    fputc('\n', f);
    printf("%s\n", links);
    free(links);

    char* type = (char*)calloc(33, sizeof(char));
    int t = dir->d_type;
    if(t == 8){
        counter[0]++;
        sprintf(type, "%s", "file");
    }
    else if(t == 4){
        counter[1]++;
        sprintf(type, "%s", "directory");
    }
    else if(t == 10){
        counter[5]++;
        sprintf(type, "%s", "symlink");
    }
    else if(t == 13){
        sprintf(type, "%s", "hardlink");
    }
    else if(t == DT_CHR){
        counter[2]++;
        sprintf(type, "%s", "character device");
    }
    else if(t == DT_BLK){
        counter[3]++;
        sprintf(type, "%s", "block device");
    }
    else if(t == DT_SOCK){
        counter[6]++;
        sprintf(type, "%s", "socket");
    }
    else if(t == DT_FIFO){
        counter[4]++;
        sprintf(type, "%s", "FIFO");
    }
    else if(t == DT_UNKNOWN){
        sprintf(type, "%s", "unknown");
    }
    else{
        sprintf(type, "%d", dir->d_type);
    }
    fputs("type: ", f);
    fputs(type, f);
    printf("%s\n", type);
    fputc('\n', f);
    free(type);

    char* size = (char*)calloc(33, sizeof(char));
    fputs("size: ", f);
    sprintf(size, "%ld", file->st_size);
    fputs(size, f);
    printf("%s\n", size);
    fputc('\n', f);
    free(size);

    char* lm = (char*)calloc(33, sizeof(char));
    fputs("last modification: ", f);
    sprintf(lm, "%ld", file->st_mtime);
    fputs(lm, f);
    printf("%s\n", lm);
    fputc('\n', f);
    free(lm);

    char* la = (char*)calloc(33, sizeof(char));
    fputs("last access: ", f);
    sprintf(la, "%ld", file->st_atime);
    fputs(la, f);
    printf("%s\n", la);
    fputc('\n', f);
    free(la);

    printf("\n");
    fputc('\n', f);
    fclose(f);
    free(file);
}

void go_deeper(char* name){
    DIR* root = opendir(name);
    struct dirent* dir = readdir(root);
    while(dir!=NULL){
        char* path = (char*)calloc(257, sizeof(char));
        strcat(path, name);
        if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
            strcat(path, "/");
            strcat(path, dir->d_name);
            if(dir->d_type == 4){
                get_info(dir, path);
                go_deeper(path);
            }
            else{
                get_info(dir, path);
            }
        }
        free(path);
        dir = readdir(root);
    }
    closedir(root);
}

int main(int argc, char* args[]){
    if(argc != 2){return 1;}
    FILE* f = fopen("./results.txt", "w");
    fclose(f);
    char* path = (char*)calloc(257, sizeof(char));
    strcat(path, args[1]);
    go_deeper(path);
    free(path);
    printf("files: %d\ndirectories: %d\ncharacter devices: %d\nblock devices: %d\nFIFO: %d\nsymbolic links: %d\nsockets: %d\n", counter[0],counter[1],counter[2],counter[3],counter[4],counter[5],counter[6]);
    return 0;
}