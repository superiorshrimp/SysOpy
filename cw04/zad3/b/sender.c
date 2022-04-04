#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>

int counter = 0;
int catcher_pid;
int n;

void sigusr1_handler(int signum, siginfo_t* info, void* ucontext){
    if(signum != SIGUSR1){return;}
    counter++;
}

void sigusr2_handler(int signum, siginfo_t* info, void* ucontext){
    if(signum != SIGUSR2){return;}
    printf("sender: otrzymano z powrotem %d sygnaly (na %d wyslane) od procesu %d\n", counter, n, catcher_pid);
}

int main(int argc, char* args[]){
    if(argc != 4){return 1;}
    catcher_pid = atoi(args[1]);
    n = atoi(args[2]);
    int type = atoi(args[3]); //0 - kill/1 - sigqueue/2 - sigrt

    struct sigaction action1;
    action1.sa_sigaction = sigusr1_handler;
    action1.sa_flags = SA_SIGINFO;
    sigemptyset(&action1.sa_mask);
    sigaction(SIGUSR1, &action1, NULL);

    struct sigaction action2;
    action2.sa_sigaction = sigusr2_handler;
    action2.sa_flags = SA_SIGINFO;
    sigemptyset(&action2.sa_mask);
    sigaction(SIGUSR2, &action2, NULL);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);

    if(type == 0){
        for(int i = 0; i<n; i++){printf("%d\n", i);
            sigset_t mask1;
            sigemptyset(&mask1);
            sigaddset(&mask1, SIGUSR1);
            kill(catcher_pid, SIGUSR1);
            sigsuspend(&mask1);
        }
        kill(catcher_pid, SIGUSR2);
    }
    else if(type == 1){
        for(int i = 0; i<n; i++){
            sigset_t mask1;
            sigemptyset(&mask1);
            sigaddset(&mask1, SIGUSR1);
            union sigval val;
            val.sival_int = i;
            sigqueue(catcher_pid, SIGUSR1, val);
            sigsuspend(&mask1);
        }
        union sigval val;
        sigqueue(catcher_pid, SIGUSR2, val);
    }
    else if(type == 2){
        for(int i = 0; i<n; i++){
            sigset_t mask1;
            sigemptyset(&mask1);
            sigaddset(&mask1, SIGUSR1);
            kill(catcher_pid, SIGRTMIN+1);
            sigsuspend(&mask1);
        }
        kill(catcher_pid, SIGRTMIN+2);
    }
    else{return 1;}
    
    sigsuspend(&mask);
    
    return 0;
}