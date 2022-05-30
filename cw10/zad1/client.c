#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "consts.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int soc;

void err(void){printf("%s\n", strerror(errno));}

void print(char* board){
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", board[0], board[1], board[2]);
    printf("| %c | %c | %c |\n", board[3], board[4], board[5]);
    printf("| %c | %c | %c |\n", board[6], board[7], board[8]);
    printf("+-----------+\n");
}

int main(int argc, char* args[]){
    //if(argc != 4){printf("argc error\n"); return 1;}
    char* name = args[1];
    //int type = atoi(args[2]);
    //char* address = args[3];

    soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    printf("socket created\n");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(connect(soc, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){printf("connect error\n"); return 1;}
    send(soc, name, NAME_LENGTH, 0);
    char response[LENGTH];
    recv(soc, &response, sizeof(response), 0);
    printf("%s", response);
    if(response[0] == 'C'){
        char status[LENGTH];
        recv(soc, status, LENGTH, 0);
        printf("%s", status);
        if(status[0] == 'W'){ //wait
            recv(soc, status, LENGTH, 0);
        }
        char* board = (char*)calloc(LENGTH, sizeof(char));
        recv(soc, board, LENGTH, 0);
        print(board);
        free(board);
        while(1){
            char* buffer = (char*)calloc(LENGTH, sizeof(char));
            recv(soc, buffer, LENGTH, 0);
            printf("%s", buffer);
            if('Y' == buffer[0]){
                char* action = (char*)calloc(LENGTH, sizeof(char));
                scanf("%s", action);
                if(strcmp(action, "dc") == 0){send(soc, action, LENGTH, 0); break;}
                else{
                    if(strlen(action) != 2 || !(action[0] == '0' || action[0] == '1' || action[0] == '2') || !(action[1] == '0' || action[1] == '1' || action[1] == '2')){printf("Wrong command!\n"); continue;}
                    send(soc, action, LENGTH, 0);
                }
                free(action);
            }
            free(buffer);
            board = (char*)calloc(LENGTH, sizeof(char));
            recv(soc, board, LENGTH, 0);
            print(board);
            free(board);
        }
        printf("Exiting\n");
    }
    close(soc);
    return 0;
}