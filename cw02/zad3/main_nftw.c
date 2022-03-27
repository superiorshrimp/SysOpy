#define _XOPEN_SOURCE 500
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
#include <ftw.h>

int counter[] = {0, 0, 0, 0, 0, 0, 0};

int get_info(const char* path, const struct stat* file, int flag, struct FTW *t){
    FILE* f = fopen("./results.txt", "a");

    fputs("path: ", f);
    fputs(path, f);
    printf("%s\n", path);
    fputc('\n', f);
    
    char* links = (char*)calloc(33, sizeof(char));
    fputs("links: ", f);
    sprintf(links, "%ld", file->st_nlink);
    fputs(links, f);
    printf("%s\n", links);
    fputc('\n', f);
    free(links);

    char* type = (char*)calloc(33, sizeof(char));
    if((file->st_mode & S_IFMT) == S_IFREG){
        counter[0]++;
        sprintf(type, "%s", "file");
    }
    else if((file->st_mode & S_IFMT) == S_IFDIR){
        counter[1]++;
        sprintf(type, "%s", "directory");
    }
    else if((file->st_mode & S_IFMT )== S_IFLNK){
        counter[5]++;
        sprintf(type, "%s", "symlink");
    } //no hardlink check?
    else if((file->st_mode & S_IFMT) == S_IFCHR){
        counter[2]++;
        sprintf(type, "%s", "character device");
    }
    else if((file->st_mode & S_IFMT) == S_IFBLK){
        counter[3]++;
        sprintf(type, "%s", "block device");
    }
    else if((file->st_mode & S_IFMT) == S_IFSOCK){
        counter[6]++;
        sprintf(type, "%s", "socket");
    }
    else if((file->st_mode & S_IFMT) == S_IFIFO){
        counter[4]++;
        sprintf(type, "%s", "FIFO");
    }
    else{
        sprintf(type, "%s", "unknown");
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
    return 0;
}

int main(int argc, char* args[]){
    if(argc != 2){return 1;}
    FILE* f = fopen("./results.txt", "w");
    fclose(f);
    nftw(args[1], get_info, 10, FTW_PHYS);
    printf("files: %d\ndirectories: %d\ncharacter devices: %d\nblock devices: %d\nFIFO: %d\nsymbolic links: %d\nsockets: %d\n", counter[0],counter[1],counter[2],counter[3],counter[4],counter[5],counter[6]);
    return 0;
}