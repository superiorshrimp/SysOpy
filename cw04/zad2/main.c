#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>

void siginfo1_handler(int signum, siginfo_t* info, void* ucontext){
    if(signum != SIGUSR1){return;}
    printf("info: \n");
    printf("    signal number: %d\n", info->si_signo);
    printf("    signal code: %d\n", info->si_code);
    printf("    signal status: %d\n", info->si_status);
    printf("    signal value: %d\n", info->si_value.sival_int);
    printf("    sent by process: %d\n", info->si_pid);
    printf("    sent by user: %d\n", info->si_uid);
}

void f1(void){
    printf("flag: SA_SIGINFO\n");
    struct sigaction action1;
    action1.sa_flags = SA_SIGINFO;
    action1.sa_sigaction = siginfo1_handler;
    sigemptyset(&action1.sa_mask);
    sigaction(SIGUSR1, &action1, NULL);
    raise(SIGUSR1);
}

void f2(void){
}

void resethand_handler(int signum, siginfo_t* info, void* ucontext){
    if(signum != SIGUSR1){return;}
    printf("info: \n");
    printf("    signal number: %d\n", info->si_signo);
    printf("    signal code: %d\n", info->si_code);
    printf("    signal status: %d\n", info->si_status);
    printf("    signal value: %d\n", info->si_value.sival_int);
    printf("    sent by process: %d\n", info->si_pid);
    printf("    sent by user: %d\n", info->si_uid);
    printf("program will close if SIGUSR1 will be raised  more time!\n");
    printf("handler for SIGUSR1 restored to default\n");
}

void f3(void){
    printf("flag: SA_RESETHAND\n");
    struct sigaction action;
    action.sa_flags = SA_RESETHAND;
    action.sa_handler = resethand_handler;
    sigemptyset(&action.sa_mask);
    sigaction(SIGUSR1, &action, NULL);
    printf("raising SIGUSR1...\n");
    raise(SIGUSR1);
    printf("raising SIGUSR1...\n");
    raise(SIGUSR1);
}

int main(int argc, char* args[]){
    f1();
    f2();
    f3();
    return 0;
}