#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

/*
example mail output:
$ mail
Heirloom mailx version 12.5 6/20/10.  Type ? for help.
"/var/mail/enlightened": 7 messages 3 unread
 O  1 Enlightened        Sat Dec  6 11:33   21/658   This is the subject
 O  2 Enlightened        Sat Dec  6 11:34  773/25549 This is the subject
 O  3 Enlightened        Sat Dec  6 16:43   20/633   This is the subject
 O  4 Enlightened        Sat Dec  6 16:44   20/633   This is the subject
 U  5 Mail Delivery Syst Sat Dec  6 16:50   74/2425  Undelivered Mail Returned to Sender
 U  6 Enlightened        Sat Dec  6 16:51   19/632   This is mutts subject
 U  7 Enlightened        Sat Dec  6 16:52   19/647   This is mutts subject
*/

void print(char* arg){
    FILE* f;
    if(strcmp(arg, "nadawca") == 0){
        f = popen("mail | tail +3 | sort -k3", "w"); //popen("cat mails.txt | tail +3 | sort -k3", "w"); //sort by column 3
    }
    else if(strcmp(arg, "data") == 0){ //default sorting
        f = popen("mail | tail +3", "w"); //popen("cat mails.txt | tail +3", "w"); //+3 to cut the header (2 lines)
    }
    else{printf("invalid method!");}
    pclose(f);
}

void send(char* to, char* about, char* content){
    FILE* f;
    char* cmd = (char*)calloc(128, sizeof(char));
    sprintf(cmd, "mail -s %s %s", about, to);
    f = popen(cmd, "w");
    fputs(content, f);
    pclose(f);
    free(cmd);
}

int main(int argc, char* args[]){
    if(argc == 2){
        print(args[1]);
    }
    else if(argc == 4){
        send(args[1], args[2], args[3]);
    }
    else{return 1;}
    return 0;
}