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
pid_t sender_pid;

void sigusr1_handler(int signum, siginfo_t* info, void* ucontext){
    if(signum != SIGUSR1){return;}
    counter++;
    sender_pid = info->si_pid;
    kill(sender_pid, SIGUSR1);
}

void sigusr2_handler(int signum, siginfo_t* info, void* ucontext){
    if(signum != SIGUSR2){return;}
    sender_pid = info->si_pid;
}

void rt1_handler(int signum, siginfo_t* info, void* ucontext){
    if(signum != SIGRTMIN+1){return;}
    counter++;
    sender_pid = info->si_pid;
    kill(sender_pid, SIGUSR1);
}

void rt2_handler(int signum, siginfo_t* info, void* ucontext){
    if(signum != SIGRTMIN+2){return;}
    sender_pid = info->si_pid;
}

int main(int argc, char* args[]){
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

    struct sigaction action3;
    action3.sa_sigaction = rt1_handler;
    action3.sa_flags = SA_SIGINFO;
    sigemptyset(&action3.sa_mask);
    sigaction(SIGRTMIN+1, &action3, NULL);

    struct sigaction action4;
    action4.sa_sigaction = rt2_handler;
    action4.sa_flags = SA_SIGINFO;
    sigemptyset(&action4.sa_mask);
    sigaction(SIGRTMIN+2, &action4, NULL);

    printf("%d\n", (int)getpid());

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGRTMIN+2);
    sigsuspend(&mask);

    int cnt = counter;
    printf("catcher: odebrano %d sygnaly od procesu %d\n", cnt, sender_pid);
    kill(sender_pid, SIGUSR2);
    return 0;
}