#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void check(void){
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending, SIGUSR1)){
        printf("SIGUSR1 pending\n");
    }
    else{
        printf("SIGUSR1 NOT pending\n");
    }
}

void handler(int signum){
    printf("Received signal!\n");
}

void check_ignore(void){
    printf("exec child: sending SIGUSR1...\n");
    raise(SIGUSR1);
    check();
    printf("exec child: sent SIGUSR1...\n");
}

void check_mask(void){
    printf("exec child: sending SIGUSR1...\n");
    signal(SIGUSR1, handler);
    raise(SIGUSR1);
    printf("exec child: sent SIGUSR1...\n");
}

void check_pending(void){
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending, SIGUSR1)){
        printf("exec child: SIGUSR1 pending\n");
    }
    else{
        printf("exec child: SIGUSR1 NOT pending\n");
    }
}

int main(int argc, char* args[]){
    char* action = args[0];
    if(strcmp(action, "ignore") == 0){
        check_ignore();
    }
    else if(strcmp(action, "mask") == 0){
        check_mask();
    }
    else if(strcmp(action, "pending") == 0){
        check_pending();
    }
    return 0;
}