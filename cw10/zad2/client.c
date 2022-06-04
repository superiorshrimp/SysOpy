#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/signal.h>

#include "consts.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int soc;
struct sockaddr_in server_address;
socklen_t len;

void exit_handler(int signum){
    send(soc, "dc", LENGTH, 0);
    close(soc);
    exit(0);
}

void err(void){printf("%s\n", strerror(errno));}

void print(char* board){
    printf("+-----------+\n");
    printf("|_%c_|_%c_|_%c_|\n", board[0], board[1], board[2]);
    printf("|_%c_|_%c_|_%c_|\n", board[3], board[4], board[5]);
    printf("|_%c_|_%c_|_%c_|\n", board[6], board[7], board[8]);
    printf("+-----------+\n");
}

void game(void){
    char status[LENGTH];
    recv(soc, status, LENGTH, 0);
    printf("%s", status); //game starting
    if(status[0] == 'Y'){ //your turn
        char* action = (char*)calloc(LENGTH, sizeof(char));
        scanf("%s", action);
        send(soc, action, LENGTH, 0);
        free(action);
    }
    else if(status[0] == 'V'){
        printf("You won, your opponent hasn't made a proper move\n");
        close(soc);
        printf("end\n");
        return;
    }
    char* buffer = (char*)calloc(LENGTH, sizeof(char));
    recv(soc, buffer, LENGTH, 0);
    printf("%s", buffer);
    free(buffer);
    char* board = (char*)calloc(LENGTH, sizeof(char));
    if(board[0] == 'V'){
        printf("You won, your opponent hasn't made a proper move.\n");
        close(soc);
        return;
    }
    recv(soc, board, LENGTH, 0);
    print(board);
    free(board);
    while(1){
        buffer = (char*)calloc(LENGTH, sizeof(char));
        recv(soc, buffer, LENGTH, 0);
        printf("%s", buffer);
        if('Y' == buffer[0]){
            char* action = (char*)calloc(LENGTH, sizeof(char));
            scanf("%s", action);
            if(strcmp(action, "dc") == 0){send(soc, action, LENGTH, 0); break;}
            else{
                if(strlen(action) != 2 || !(action[0] == '0' || action[0] == '1' || action[0] == '2') || !(action[1] == '0' || action[1] == '1' || action[1] == '2')){send(soc, "dc", LENGTH, 0); free(action); return; }
                else{send(soc, action, LENGTH, 0);}
            }
            free(action);
        }
        else if(buffer[0] == 'V'){
            printf("You won, your opponent hasn't made a proper move.\n");
            close(soc);
            return;
        }
        free(buffer);
        board = (char*)calloc(LENGTH, sizeof(char));
        recv(soc, board, LENGTH, 0);
        if(board[0] == 'V'){
            printf("You won, your opponent hasn't made a proper move.\n");
            close(soc);
            free(board);
            return;
        }
        print(board);
        free(board);
    }
}

int main(int argc, char* args[]){
    signal(SIGINT, exit_handler);
    char* name = args[1];

    soc = socket(AF_INET, SOCK_DGRAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    printf("socket created\n");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    socklen_t len = sizeof(server_address);
    connect(soc, (struct sockaddr*)&server_address, len);

    len = sizeof(server_address);
    char* login = (char*)calloc(LENGTH, sizeof(char));
    login[0] = 'l';
    strcat(login, name);
    send(soc, login, LENGTH, 0);
    printf("Trying to connect to the server\n");
    free(login);
    
    char response[LENGTH];
    recv(soc, &response, LENGTH, 0);
    printf("%s", response);
    if(response[0] == 'C'){
        printf("Connected to the server\n");
        game();
    }
    else if(response[0] == 'T'){
        printf("Too many clients!\n");
    }
    else{
        printf("Player with the same name already on the server!\n");
    }
    close(soc);
    return 0;
}