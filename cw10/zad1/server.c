#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "consts.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int soc;
int clients_count = 0;
int clients_active[CLIENTS];
char* clients_name[CLIENTS];
int matches[CLIENTS];
int playing[CLIENTS];
int matches_count = 0;

typedef struct argz{
    int* a;
    int* b;
} argz;

void err(void){printf("%s\n", strerror(errno));}

char* parse_board(int** board){
    char* ret = (char*)calloc(LENGTH, sizeof(char));
    int counter = 0;
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            if(board[i][j] == 0){
                ret[counter] = 'o';
            }
            else if(board[i][j] == 1){
                ret[counter] = 'x';
            }
            else{
                ret[counter] = ' ';
            }
            counter++;
        }
    }
    return ret;
}

int game(int** tab, int type, int x, int y){ //type: 0 - circle
    if(tab[y][x] == 2){
        tab[y][x] = type;
    }
    else{
        return -1; //error
    }
    for(int y = 0; y<3; y++){
        if(tab[y][0] == type && tab[y][1] == type && tab[y][2] == type){return 1;}
    }
    for(int x = 0; x<3; x++){
        if(tab[0][x] == type && tab[1][x] == type && tab[2][x] == type){return 1;}
    }
    if(tab[0][0] == type && tab[1][1] == type && tab[2][2] == type){return 1;}
    if(tab[2][0] == type && tab[1][1] == type && tab[0][2] == type){return 1;}
    return 0;
}

void* handle_connect(void* args){
    listen(soc, CLIENTS);
    while(1){
        int client_soc = accept(soc, NULL, NULL);
        if(client_soc == -1){printf("connect error\n"); exit(1);}
        else{
            if(clients_count >= CLIENTS){
                char message[LENGTH] = "Too many players on the server!\n";
                send(client_soc, message, sizeof(message), 0);
            }
            else{
                clients_count++;
                for(int i = 0; i<CLIENTS; i++){
                    if(clients_active[i] == -1){
                        clients_active[i] = client_soc;
                        char* buffer = (char*)calloc(NAME_LENGTH, sizeof(char));
                        recv(client_soc, buffer, NAME_LENGTH, 0);
                        int flag = 1;
                        for(int j = 0; j<CLIENTS; j++){
                            if(clients_active[j] >= 0 && strcmp(clients_name[j], buffer) == 0){
                                flag = 0;
                                break;
                            }
                        }
                        if(flag == 0){
                            char message[LENGTH] = "Player with the same name already here!\n";
                            send(client_soc, message, sizeof(message), 0);
                            break;
                        }
                        else{
                            clients_name[i] = buffer;
                            printf("Client connected with %s\n", buffer);
                            if(clients_count%2 == 1){
                                char message[LENGTH] = "Connected!\n";
                                send(client_soc, message, sizeof(message), 0);
                                char action[LENGTH] = "Wait for another player.\n";
                                send(client_soc, action, sizeof(message), 0);
                            }
                            else{
                                char message[LENGTH] = "Connected!\n";
                                send(client_soc, message, sizeof(message), 0);
                                char action[LENGTH] = "Game starting!\n";
                                send(client_soc, action, sizeof(message), 0);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

void* handle_game(void* args){
    argz* argz = args;
    printf("%d %d\n", *argz->a, *argz->b);
    return NULL;
}

void* handle_main(void* args){
    pthread_t* games = (pthread_t*)calloc((int)(CLIENTS/2) + CLIENTS%2, sizeof(pthread_t));
    argz** arguments = (argz**)calloc((int)(CLIENTS/2) + CLIENTS%2, sizeof(argz*));
    while(1){
        if(matches_count < clients_count){ //TODO: mutexy
            for(int i = 0; i<CLIENTS; i++){
                for(int j = 0; j<CLIENTS; j++){
                    if(i != j && clients_active[i] >= 0 && playing[i]<0 && clients_active[j] >= 0 && playing[j]<0){
                        printf("a %d %d\n", i ,j);
                        playing[i] = j;
                        playing[j] = i;
                        printf("Starting game %s vs %s\n", clients_name[i], clients_name[j]);
                        arguments[(int)(matches_count/2)] = (argz*)calloc(1,sizeof(argz));
                        arguments[(int)(matches_count/2)]->a = (int*)calloc(1,sizeof(int));
                        arguments[(int)(matches_count/2)]->b = (int*)calloc(1,sizeof(int));
                        *arguments[(int)(matches_count/2)]->a = i;
                        *arguments[(int)(matches_count/2)]->b = j;
                        pthread_create(&games[(int)(matches_count/2)], NULL, handle_game, arguments);
                        matches_count += 2;
                    }
                }
            }
        }
        for(int i = 0; i<(int)(matches_count/2); i++){
            pthread_join(games[i], NULL);
        }
        /*for(int i = 0; i<(int)(matches_count/2); i++){
            free(arguments[i]);
        }
        free(arguments);
        free(games);*/
    }
    /*
    //na razie 2 graczy
    int** board = (int**)calloc(3, sizeof(int*));
    for(int i = 0; i<3; i++){
        board[i] = (int*)calloc(3, sizeof(int));
        for(int j = 0; j<3; j++){board[i][j] = 2;}
    }
    while(clients_active[0] < 0 || clients_active[1] <0){
        sleep(3);
    }
    send(clients_active[0], "Game starting\n", LENGTH, 0);
    char* b = parse_board(board);
    send(clients_active[0], b, LENGTH, 0);
    send(clients_active[1], b, LENGTH, 0);
    send(clients_active[0], "Your turn\n", LENGTH, 0);
    send(clients_active[1], "Wait for opponent's move\n", LENGTH, 0);
    free(b);
    for(int round = 0; round<9; round++){
        char* buffer = (char*)calloc(LENGTH, sizeof(char));
        recv(clients_active[round%2], buffer, LENGTH, 0);
        int res = game(board, round%2, (int)(atoi(buffer)/10), (int)(atoi(buffer)%10));
        free(buffer);
        b = parse_board(board);
        send(clients_active[round%2], b, LENGTH, 0);
        send(clients_active[(round+1)%2], b, LENGTH, 0);
        send(clients_active[(round+1)%2], "Your turn\n", LENGTH, 0);
        send(clients_active[round%2], "Wait for opponent's move\n", LENGTH, 0);
        free(b);
        if(res == -1){printf("error\n");break;}
        else if(res == 1){break;}
    }

    for(int i = 0; i<3; i++){
        free(board[i]);
    }
    free(board);
    */
    return NULL;
}

int main(int argc, char* args[]){
    //if(argc != 3){printf("argc error\n"); return 1;}
    //int port = atoi(args[1]);
    //char* path = args[2];

    for(int i = 0; i<CLIENTS; i++){
        clients_active[i] = -1;
        clients_name[i] = "\0";
        matches[i] = -1;
        playing[i] = -1;
    }

    soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    printf("socket created\n");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    bind(soc, (struct sockaddr*)&server_address, sizeof(server_address));

    pthread_t connect_thread;
    pthread_t main_thread;
    pthread_create(&connect_thread, NULL, &handle_connect, NULL);
    pthread_create(&main_thread, NULL, &handle_main, NULL);
    pthread_join(connect_thread, NULL);
    pthread_join(main_thread, NULL);
    close(soc);
    return 0;
}