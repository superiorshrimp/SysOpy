#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

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
    signal(SIGUSR1, SIG_IGN);
    printf("parent: sending SIGUSR1...\n");
    raise(SIGUSR1);
    check();
    printf("parent: sent SIGUSR1...\n");
    pid_t child_pid = fork();
    if(child_pid == 0){ //child
        sleep(1); //for clean output
        printf("child: sending SIGUSR1...\n");
        raise(SIGUSR1);
        check();
        printf("child: sent SIGUSR1...\n");
        exit(0);
    }
    else{
        wait(NULL);
        execl("./main_exec", "ignore", NULL);
    }
}

void check_handler(void){
    signal(SIGUSR1, handler);
    printf("parent: sending SIGUSR1...\n");
    raise(SIGUSR1);
    printf("parent: sent SIGUSR1...\n");
    pid_t child_pid = fork();
    if(child_pid == 0){ //child
        sleep(1); //for clean output
        printf("child: sending SIGUSR1...\n");
        raise(SIGUSR1);
        printf("child: sent SIGUSR1...\n");
        exit(0);
    }
    else{
        wait(NULL);
    }
}

void check_mask(void){
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    signal(SIGUSR1, handler);
    printf("parent: sending SIGUSR1...\n");
    raise(SIGUSR1);
    signal(SIGUSR1, handler);
    printf("parent: sent SIGUSR1...\n");
    pid_t child_pid = fork();
    if(child_pid == 0){ //child
        sleep(1); //for clean output
        printf("child: sending SIGUSR1...\n");
        raise(SIGUSR1);
        signal(SIGUSR1, handler);
        printf("child: sent SIGUSR1...\n");
        exit(0);
    }
    else{
        wait(NULL);
        execl("./main_exec", "mask", NULL);
    }
}

void check_pending(void){
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    printf("parent: sending SIGUSR1...\n");
    raise(SIGUSR1);
    printf("parent: sent SIGUSR1...\n");
    pid_t child_pid = fork();
    if(child_pid == 0){ //child
        sleep(1); //for clean output
        sigset_t pending;
        sigpending(&pending);
        if(sigismember(&pending, SIGUSR1)){
            printf("child: SIGUSR1 pending\n");
        }
        else{
            printf("child: SIGUSR1 NOT pending\n");
        }
        exit(0);
    }
    else{
        wait(NULL);
        execl("./main_exec", "pending", NULL);
    }
}

int main(int argc, char* args[]){
    if(argc != 2){return 1;}
    char* action = args[1];
    if(strcmp(action, "ignore") == 0){
        printf("Checking ignore:\n");
        check_ignore();
        printf("End of checking ignore\n");
    }
    else if(strcmp(action, "handler") == 0){
        printf("Checking handler:\n");
        check_handler();
        printf("End of checking handler\n");
    }
    else if(strcmp(action, "mask") == 0){
        printf("Checking mask:\n");
        check_mask();
        printf("End of checking mask\n");
    }
    else if(strcmp(action, "pending") == 0){
        printf("Checking pending:\n");
        check_pending();
        printf("End of checking pending\n");
    }
    else{return 1;}
    printf("\n");
    return 0;
}