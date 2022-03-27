#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* args[]){
    if(argc != 2){return 1;}
    int n = atoi(args[1]);
    for(int i = 0; i<n; i++){
        pid_t child_pid = vfork();
        if(child_pid == 0){ //child
            printf("%d. proces potomny; rodzic: %d\n", i, (int)getppid());
            exit(0);
        }
        else{ //waits for child process to end
            wait(NULL);
        }
    }
    return 0;
}