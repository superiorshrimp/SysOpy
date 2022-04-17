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
#include <time.h>

int main(int argc, char* args[]){
    if(argc != 4){return 1;}
    char* path = args[0]; //fifo
    char* name = args[1]; //row nr
    char* data = args[2];
    int N = atoi(args[3]);

    FILE* f = fopen(data, "r");
    int fd = open(path, O_WRONLY);
    if(fd == -1){return 1;}

    char* buffer = (char*)calloc(N+2, sizeof(char));
    buffer[0] = name[0];

    srand((unsigned int)time(NULL));

    for(int i = 0; i<5; i++){
        for(int j = 0; j<N; j++){
            buffer[j+1] = fgetc(f);
        }
        float cooldown = ((float)rand())/RAND_MAX * 1000000.0;
        usleep(cooldown);
        write(fd, buffer, N+1);
        free(buffer);
        buffer = (char*)calloc(N+2, sizeof(char));
        buffer[0] = name[0];
    }

    free(buffer);
    fclose(f);
    close(fd);
    
    return 0;
}