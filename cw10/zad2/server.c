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

int soc;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int clients_count = 0;
int clients_active[CLIENTS];
char* clients_name[CLIENTS];
struct sockaddr clients_address[CLIENTS];
pthread_mutex_t matches_mutex = PTHREAD_MUTEX_INITIALIZER;
int playing[CLIENTS];
int games[CLIENTS];
int types[CLIENTS];
int rounds[CLIENTS];
int** boards[CLIENTS];

void exit_handler(int signum){
    for(int i = 0; i<CLIENTS; i++){
        if(clients_active[i] >= 0){
            free(clients_name[i]);
        }
    }
    close(soc);
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

void handle_dc(struct sockaddr client){
    for(int i = 0; i<CLIENTS; i++){
        if(memcmp(&clients_address[i], &client, sizeof(struct sockaddr)) == 0){
            clients_active[i] = -1;
            clients_count--;
            printf("%s disconnected\n", clients_name[i]);
            clients_name[i] = "\0";
            break;
        }
    }
}

void handle_connect(struct sockaddr client, socklen_t len, char* buffer){
    char* name = (char*)calloc(LENGTH, sizeof(char));
    strncpy(name, buffer+1, strlen(buffer)-1);
    if(clients_count >= CLIENTS){
        sendto(soc, "Too many players on the server!\n", LENGTH, 0, (struct sockaddr*)&client, len);
    }
    else{
        for(int i = 0; i<CLIENTS; i++){
            if(clients_active[i] < 0){
                int flag = 0;
                for(int j = 0; j<CLIENTS; j++){
                    if(strcmp(clients_name[j], name) == 0){
                        flag = 1;
                    }
                }
                if(flag == 1){
                    sendto(soc, "Player with the same name already here!\n", LENGTH, 0, (struct sockaddr*)&client, len);
                }
                else{
                    clients_count++;
                    clients_active[i] = 1;
                    clients_address[i] = client;
                    clients_name[i] = name;
                    sendto(soc, "Connected\n", LENGTH, 0, (struct sockaddr*)&client, len);
                    if(clients_count%2 == 1){
                        sendto(soc, "Wait for another player.\n", LENGTH, 0, (struct sockaddr*)&client, len);
                    }
                    else{
                        sendto(soc, "Game starting!\n", LENGTH, 0, (struct sockaddr*)&client, len);
                    }
                    printf("Connected %s!\n", name);
                }
                break;
            }
        }
    }
}

void handle_matches(socklen_t len){
    pthread_mutex_lock(&matches_mutex);
    int f = 0;
    for(int i = 0; i<CLIENTS; i++){
        if(clients_active[i] < 0 || playing[i] >= 0){continue;}
        for(int j = 0; j<CLIENTS; j++){
            if(i == j || clients_active[j] < 0 || playing[j] >= 0){continue;}
            playing[i] = j;
            playing[j] = i;
            printf("Starting game %s vs %s\n", clients_name[i], clients_name[j]);
            types[i] = 0;
            types[j] = 1;
            games[i] = i;
            games[j] = i;
            boards[i] = (int**)calloc(3, sizeof(int*));
            for(int k = 0; k<3; k++){
                boards[i][k] = (int*)calloc(3, sizeof(int));
                for(int l = 0; l<3; l++){boards[i][k][l] = 2;}
            }
            rounds[i] = 1;
            sendto(soc, "Game starting\n", LENGTH, 0, (struct sockaddr*)&clients_address[i], len);
            sendto(soc, "Your opponent starts\n", LENGTH, 0, (struct sockaddr*)&clients_address[j], len);
            char* b = parse_board(boards[i]);
            sendto(soc, b, LENGTH, 0, (struct sockaddr*)&clients_address[i], len);
            sendto(soc, b, LENGTH, 0, (struct sockaddr*)&clients_address[j], len);
            free(b);
            sendto(soc, "Your move:\n", LENGTH, 0, (struct sockaddr*)&clients_address[i], len);
            sendto(soc, "Wait for your opponent's move\n", LENGTH, 0, (struct sockaddr*)&clients_address[j], len);

            f = 1;
            break;
        }
        if(f == 1){break;}
    }
    pthread_mutex_unlock(&matches_mutex);
}

void end(int id, int i, int j){
    clients_count -= 2;
    clients_active[i] = -1;
    clients_active[j] = -1;
    clients_name[i] = '\0';
    clients_name[j] = '\0';
    playing[i] = -1;
    playing[j] = -1;
    games[id] = -1;
    for(int k = 0; k<3; k++){free(boards[id][k]);}
    free(boards[id]);
}

void handle_game(struct sockaddr client, socklen_t len, char* buffer){
    int id;
    int game_id;
    for(int i = 0; i<CLIENTS; i++){
        if(memcmp(&clients_address[i], &client, sizeof(struct sockaddr)) == 0){
            id = i;
            game_id = games[i];
            break;
        }
    }
    int y = buffer[0]-'0';
    int x = buffer[1]-'0';

    int res = game(boards[game_id], types[id], y, x);
    
    char* b = parse_board(boards[game_id]);
    sendto(soc, b, LENGTH, 0, (struct sockaddr*)&client, len);
    sendto(soc, b, LENGTH, 0, (struct sockaddr*)&clients_address[playing[id]], len);
    free(b);
    
    if(res == -1){ //mistake
        sendto(soc, "Victory! Your opponent has made a misstake.\n", LENGTH, 0, (struct sockaddr*)&clients_address[playing[id]], len);
        end(game_id, id, playing[id]);
        return;
    }
    else if(res == 1){
        sendto(soc, "You won!\n", LENGTH, 0, (struct sockaddr*)&client, len);
        sendto(soc, "You lost!\n", LENGTH, 0, (struct sockaddr*)&clients_address[playing[id]], len);
        printf("Player %s won!\n", clients_name[id]);
        end(game_id, id, playing[id]);
        return;
    }
    else{
        if(rounds[game_id] != 9){
            sendto(soc, "Your move:\n", LENGTH, 0, (struct sockaddr*)&clients_address[playing[id]], len);
            sendto(soc, "Wait for your opponent's move\n", LENGTH, 0, (struct sockaddr*)&client, len);
        }
        else{
            sendto(soc, "Draw!\n", LENGTH, 0, (struct sockaddr*)&client, len);
            sendto(soc, "Draw!\n", LENGTH, 0, (struct sockaddr*)&clients_address[playing[id]], len);
            printf("Draw (%s - %s)\n", clients_name[id], clients_name[playing[id]]);
            end(game_id, id, playing[id]);
            return;
        }
    }
    rounds[game_id]++;
}

void* handle_main(void* args){
    while(1){
        struct sockaddr client;
        socklen_t len = sizeof(client);
        pthread_mutex_lock(&clients_mutex);
        char* buffer = (char*)calloc(LENGTH, sizeof(char));
        int n = recvfrom(soc, buffer, LENGTH, 0, (struct sockaddr*)&client, &len);
        buffer[n] = '\0';

        if(buffer[0] == 'l'){ //login
            handle_connect(client, len, buffer);
            handle_matches(len);
        }
        else if(buffer[0] == 'd'){ //dc
            handle_dc(client);
        }
        else{ //move
            handle_game(client, len, buffer);
        }
        free(buffer);

        pthread_mutex_unlock(&clients_mutex);
    }
    return NULL;
}

int main(int argc, char* args[]){
    signal(SIGINT, exit_handler);

    for(int i = 0; i<CLIENTS; i++){
        clients_active[i] = -1;
        clients_name[i] = "\0";
        playing[i] = -1;
    }

    soc = socket(AF_INET, SOCK_DGRAM, 0);
    if(soc == -1){printf("socket error\n"); return 1;}
    printf("socket created\n");
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(soc, (struct sockaddr*)&server_address, sizeof(server_address));

    pthread_t main_thread;
    pthread_create(&main_thread, NULL, &handle_main, NULL);
    pthread_join(main_thread, NULL);
    close(soc);
    return 0;
}