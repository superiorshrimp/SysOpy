#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/signal.h>

#include "consts.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

int soc; //web socket
int lsoc; //local socket
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clients_cond = PTHREAD_COND_INITIALIZER;
int clients_count = 0;
int clients_active[CLIENTS];
char* clients_name[CLIENTS];
pthread_mutex_t matches_mutex = PTHREAD_MUTEX_INITIALIZER;
int playing[CLIENTS];

typedef struct argz{
    int* a;
    int* b;
} argz;

void exit_handler(int signum){
    close(soc);
    close(lsoc);
    exit(0);
}

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

int game(int** tab, int type, int y, int x){ //type: 0 - circle
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
    listen(soc, CLIENTS+1);
    listen(lsoc, CLIENTS+1);
    while(1){
        int client_soc = accept(soc, NULL, NULL);
        int client_lsoc = accept(lsoc, NULL, NULL);
        if(client_soc == -1){client_soc = client_lsoc;}
        if(client_soc != -1){
            pthread_mutex_lock(&clients_mutex);
            if(clients_count >= CLIENTS){
                char message[LENGTH] = "Too many players on the server!\n";
                send(client_soc, message, sizeof(message), 0);
                shutdown(client_soc, SHUT_RDWR);
            }
            else{
                char* buffer = (char*)calloc(NAME_LENGTH, sizeof(char));
                recv(client_soc, buffer, NAME_LENGTH, 0);
                int flag = 0;
                for(int i = 0; i<CLIENTS; i++){
                    if(clients_active[i] >= 0 && strcmp(clients_name[i], buffer) == 0){
                        char message[LENGTH] = "Player with the same name already here!\n";
                        send(client_soc, message, sizeof(message), 0);
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0){
                    for(int i = 0; i<CLIENTS; i++){
                        if(clients_active[i] == -1){
                            clients_count++;
                            clients_active[i] = client_soc;
                            clients_name[i] = buffer;
                            printf("Client connected with %s\n", buffer);
                            pthread_cond_broadcast(&clients_cond);
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
            pthread_mutex_unlock(&clients_mutex);
        }
    }
    return NULL;
}

void* handle_game(void* args){
    argz* argz = args;
    pthread_mutex_lock(&clients_mutex);
    int p1_soc = clients_active[*argz->a];
    int p2_soc = clients_active[*argz->b];
    pthread_mutex_unlock(&clients_mutex);

    int** board = (int**)calloc(3, sizeof(int*));
    for(int i = 0; i<3; i++){
        board[i] = (int*)calloc(3, sizeof(int));
        for(int j = 0; j<3; j++){board[i][j] = 2;}
    }

    send(p1_soc, "Game starting\n", LENGTH, 0);
    send(p2_soc, "Your opponent starts\n", LENGTH, 0);
    char* b = parse_board(board);
    send(p1_soc, b, LENGTH, 0);
    send(p2_soc, b, LENGTH, 0);
    free(b);
    send(p1_soc, "Your move\n", LENGTH, 0);
    send(p2_soc, "Wait for opponent's move\n", LENGTH, 0);
    for(int round = 0; round<9; round++){
        char* buffer = (char*)calloc(LENGTH, sizeof(char));
        if(round%2 == 0){recv(p1_soc, buffer, LENGTH, 0);}
        else{recv(p2_soc, buffer, LENGTH, 0);}
        if(buffer[0] == 'd'){ //dc
            pthread_mutex_lock(&clients_mutex);
            pthread_mutex_lock(&matches_mutex);
            shutdown(clients_active[*argz->a], SHUT_RDWR);
            shutdown(clients_active[*argz->b], SHUT_RDWR);
            clients_count -= 2;
            clients_active[*argz->a] = -1;
            clients_name[*argz->a] = '\0';
            playing[*argz->a] = -1;
            clients_active[*argz->b] = -1;
            clients_name[*argz->b] = '\0';
            playing[*argz->b] = -1;
            pthread_cond_broadcast(&clients_cond);
            pthread_mutex_unlock(&matches_mutex);
            pthread_mutex_unlock(&clients_mutex);
            if(round%2 == 0){
                send(p2_soc, "Victory! Your opponent has left.\n", LENGTH, 0);
            }
            else{
                send(p1_soc, "Victory! Your opponent has left.\n", LENGTH, 0);
            }
            break;
            free(buffer);
        }
        int res = game(board, round%2, (int)(atoi(buffer)/10), (int)(atoi(buffer)%10));
        free(buffer);
        b = (char*)calloc(LENGTH, sizeof(char));
        b = parse_board(board);
        if(res == -1){ //invalid move
            if(round%2 == 0){
                send(p2_soc, "Victory! Your opponent has made a misstake.\n", LENGTH, 0);
            }
            else{
                send(p1_soc, "Victory! Your opponent has made a misstake.\n", LENGTH, 0);
            }
        }
        else if(res == 0){
            if(round == 8){
                send(p1_soc, b, LENGTH, 0);
                send(p2_soc, b, LENGTH, 0);
                send(p1_soc, "Draw!\n", LENGTH, 0);
                send(p2_soc, "Draw!\n", LENGTH, 0);
                printf("Draw (%s - %s)\n", clients_name[*argz->a], clients_name[*argz->b]);
                break;
            }
            send(p1_soc, b, LENGTH, 0);
            send(p2_soc, b, LENGTH, 0);
            if(round%2 == 1){
                send(p1_soc, "Your move:\n", LENGTH, 0);
                send(p2_soc, "Wait for your opponent's move\n", LENGTH, 0);
            }
            else{
                send(p2_soc, "Your move:\n", LENGTH, 0);
                send(p1_soc, "Wait for your opponent's move\n", LENGTH, 0);
            }
        }
        else{ //won
            if(round%2 == 0){
                send(p1_soc, b, LENGTH, 0);
                send(p2_soc, b, LENGTH, 0);
                send(p1_soc, "You won!\n", LENGTH, 0);
                send(p2_soc, "You lost!\n", LENGTH, 0);
                printf("Player %s won!\n", clients_name[*argz->a]);
                free(b);
                break;
            }
            else{
                send(p1_soc, b, LENGTH, 0);
                send(p2_soc, b, LENGTH, 0);
                send(p2_soc, "You won!\n", LENGTH, 0);
                send(p1_soc, "You lost!\n", LENGTH, 0);
                printf("Player %s won!\n", clients_name[*argz->b]);
                free(b);
                break;
            }
        }
        free(b);
    }
    for(int i = 0; i<3; i++){free(board[i]);}
    free(board);
    return NULL;
}

void* handle_main(void* args){
    pthread_t* games = (pthread_t*)calloc(CLIENTS, sizeof(pthread_t));
    argz** arguments = (argz**)calloc(CLIENTS, sizeof(argz*));
    int* allocated = (int*)calloc(CLIENTS, sizeof(int));
    for(int i = 0; i<CLIENTS; i++){
        allocated[i] = 0;
    }
    while(1){
        pthread_mutex_lock(&clients_mutex);
        pthread_cond_wait(&clients_cond, &clients_mutex);
        pthread_mutex_lock(&matches_mutex);

        for(int i = 0; i<CLIENTS; i++){
            if(playing[i] >= 0 && clients_active[i] < 0 && allocated[i] == 1){
                playing[i] = -1;
                free(arguments[i]->a);
                free(arguments[i]->b);
                free(arguments[i]);
                allocated[i] = 0;
            }
        }

        int f = 0;
        for(int i = 0; i<CLIENTS; i++){
            if(clients_active[i] < 0 || playing[i] >= 0){continue;}
            for(int j = 0; j<CLIENTS; j++){
                if(i == j || clients_active[j] < 0 || playing[j] >= 0){continue;}
                playing[i] = j;
                playing[j] = i;
                printf("Starting game %s vs %s\n", clients_name[i], clients_name[j]);
                arguments[i] = (argz*)calloc(1,sizeof(argz));
                arguments[i]->a = (int*)calloc(1,sizeof(int));
                arguments[i]->b = (int*)calloc(1,sizeof(int));
                *arguments[i]->a = i;
                *arguments[i]->b = j;
                pthread_create(&games[i], NULL, handle_game, arguments[i]);
                f = 1;
                break;
            }
            if(f == 1){break;}
        }

        pthread_mutex_unlock(&matches_mutex);
        pthread_mutex_unlock(&clients_mutex);
    }
    free(allocated);
    free(games);
    free(arguments);
    return NULL;
}

int main(int argc, char* args[]){
    if(argc != 3){return 1;}
    int port = atoi(args[1]);
    char* path = args[2];
    if(port == 0){port = PORT;}
    if(path[0] == '0'){path = PATH;}

    signal(SIGINT, exit_handler);

    for(int i = 0; i<CLIENTS; i++){
        clients_active[i] = -1;
        clients_name[i] = "\0";
        playing[i] = -1;
    }

    soc = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    printf("socket created\n");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    bind(soc, (struct sockaddr*)&server_address, sizeof(server_address));

    lsoc = socket(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(lsoc == -1){printf("local socket error\n"); return 1;}
    printf("local socket created\n");

    struct sockaddr_un lserver_address;
    strcpy(lserver_address.sun_path, path);
    lserver_address.sun_family = AF_LOCAL;
    bind(lsoc, (struct sockaddr*)&lserver_address, sizeof(lserver_address));

    pthread_t connect_thread;
    pthread_t main_thread;
    pthread_create(&connect_thread, NULL, &handle_connect, NULL);
    pthread_create(&main_thread, NULL, &handle_main, NULL);
    pthread_join(connect_thread, NULL);
    pthread_join(main_thread, NULL);
    close(soc);
    close(lsoc);
    return 0;
}