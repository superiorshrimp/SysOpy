#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

int main(int argc, char* args[]){
    if(mkfifo("fifo", 0777) == -1){
        printf("error while creating fifo\n");
    }

    for(int i = 0; i<3; i++){
        if(fork() == 0){
            char* path = (char*)calloc(128, sizeof(char));
            sprintf(path, "./data/d%d.txt" , i);
            char* name = (char*)calloc(2, sizeof(char));
            sprintf(name, "%d" , i);

            execl("./prod", "./fifo", name, path, "5", NULL);

            free(path);
            free(name);
            exit(0);
        }
    }

    execl("./kons", "./fifo", "./result.txt", "5", NULL);
    
    return 0;
}