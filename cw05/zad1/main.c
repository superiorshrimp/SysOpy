#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

typedef struct command{
    char* program;
    char** args;
    int len;
} Command;

typedef struct component{
    char* name;
    Command** commands;
    int len;
} Component;

typedef struct sequence{
    Component** components;
    int len;
} Sequence;

Component* get_component(char* name, Component** arr, int len){
    for(int i = 0; i<len; i++){
        if(strcmp(name, arr[i]->name) == 0){return arr[i];}
    }
    return NULL;
}

void free_memory(Component** arr, int components_count, Sequence** todo, int seq_count){
    for(int i = 0; i<components_count; i++){
        free(arr[i]->name);
        for(int j = 0; j<arr[i]->len; j++){
            free(arr[i]->commands[j]->program);
            for(int k = 0; k<arr[i]->commands[j]->len; k++){
                free(arr[i]->commands[j]->args[k]);
            }
            free(arr[i]->commands[j]->args);
            free(arr[i]->commands[j]);
        }
        free(arr[i]->commands);
        free(arr[i]);
    }
    free(arr);

    for(int i = 0; i<seq_count; i++){
        free(todo[i]->components);
        free(todo[i]);
    }
    free(todo);
}

void create_sequences(char* file, Sequence** todo, int seq_count, int components_count, Component** arr){
    FILE* f = fopen(file, "r");
    int line = 0;
    while(line != components_count + 1){
        char c = fgetc(f);
        if(c == '\n'){
            line++;
        }
    }

    int i = 0;
    int j = 0;
    char* buffer = (char*)calloc(128, sizeof(char));
    todo[0] = (Sequence*)calloc(1,sizeof(Sequence));;
    todo[0]->components = (Component**)calloc(components_count, sizeof(Component*));
    int k = 0;
    while(i != seq_count){
        char c = fgetc(f);
        if(c == '\n' || c == EOF){
            todo[i]->components[k] = get_component(buffer, arr, components_count);
            todo[i]->len = k + 1;
            i++;
            free(buffer);
            if(i != seq_count){
                todo[i] = (Sequence*)calloc(1,sizeof(Sequence));
                todo[i]->components = (Component**)calloc(components_count, sizeof(Component*));
                buffer = (char*)calloc(128, sizeof(char));
                j = -1;
                k = 0;
            }
        }
        else if(c == '|'){
            todo[i]->components[k] = get_component(buffer, arr, components_count);
            free(buffer);
            buffer = (char*)calloc(128, sizeof(char));
            j = -2;
            k++;
        }
        else if(c != ' '){
            buffer[j] = c;
        }
        j++;
    }
    fclose(f);
}

int count_components(char* file){
    FILE* f = fopen(file, "r");
    char c = fgetc(f);
    int count = 0;
    while(c != EOF){
        if(c == '='){
            count++;
        }
        c = fgetc(f);
    }
    fclose(f);
    return count;
}

int count_sequences(char* file, int components_count){
    FILE* f = fopen(file, "r");
    char c = fgetc(f);
    int count = 0;
    while(c != EOF){
        if(c == '\n'){
            count++;
        }
        c = fgetc(f);
    }
    fclose(f);
    return count - components_count;
}

void create_components(char* name, Component** arr, int len){ //i assume that components' names have no spaces inbetween
    FILE* f = fopen(name, "r");
    int i = 0;
    int j = 0;
    int flag = 1; //before '='
    char* buffer = (char*)calloc(128, sizeof(char));
    while(i != len){
        char c = fgetc(f);
        if(c == '\n'){
            i++;
            flag = 1;
            free(buffer);
            buffer = (char*)calloc(128, sizeof(char));
            j = -1;
        }
        else if(c == '='){
            arr[i] = (Component*)calloc(1,sizeof(Component));
            flag = 0;
            arr[i]->name = (char*)calloc(128, sizeof(char));
            strcpy(arr[i]->name, buffer);
        }
        else if(flag == 1 && c != ' '){
            buffer[j] = c;
        }
        j++;
    }
    free(buffer);
    fclose(f);
}

Command* create_command(char* content, int j){
    Command* cmd = (Command*)calloc(1,sizeof(Command));
    int flag = 0; //after name
    char* buffer = (char*)calloc(64, sizeof(char));
    int k = 0;
    for(int i = 0; i<j; i++){
        char c = content[i];
        if(c == ' '){
            k = -1;
            if(flag == 0){
                cmd->len = 0;
                cmd->program = (char*)calloc(64, sizeof(char));
                strcpy(cmd->program, buffer);
                flag = 1;
                cmd->args = (char**)calloc(32, sizeof(char*));
                free(buffer);
                buffer = (char*)calloc(64, sizeof(char));
            }
            else{
                cmd->args[cmd->len] = (char*)calloc(128, sizeof(char));
                strcpy(cmd->args[cmd->len], buffer);
                cmd->len++;
                free(buffer);
                buffer = (char*)calloc(64, sizeof(char));
                //k--; //czy potrzebne?
            }
        }
        else{
            buffer[k] = c;
        }
        k++;
    }
    if(flag == 0){
        cmd->program = (char*)calloc(64, sizeof(char));
        strcpy(cmd->program, buffer);
        cmd->args = (char**)calloc(32, sizeof(char*));
        cmd->args[0] = (char*)calloc(128, sizeof(char));
        strcpy(cmd->args[cmd->len], buffer);
        cmd->len = 1;
        free(buffer);
    }
    else{
        cmd->args[cmd->len] = (char*)calloc(128, sizeof(char));
        strcpy(cmd->args[cmd->len], buffer);
        cmd->len++;
        free(buffer);
    }
    return cmd;
}

Command** rewrite(Command** cmds, int len){
    Command** ret = (Command**)calloc(len, sizeof(Command*));
    for(int i = 0; i<len; i++){
        ret[i] = cmds[i];
    }
    free(cmds);
    return ret;
}

void create_commands(char* name, Component** arr, int len){
    FILE* f = fopen(name, "r");
    int i = 0;
    int j = 0;
    int k = 0;
    int flag = 0; //after '='
    char* buffer = (char*)calloc(512, sizeof(char));
    Command** cmds = (Command**)calloc(32, sizeof(Command*));
    while(i != len){
        char c = fgetc(f);
        if(c == '\n'){
            arr[i]->len = k + 1;
            flag = 0;
            cmds[k] = create_command(buffer, j+1);
            arr[i]->commands = rewrite(cmds, k+1); //frees cmds
            cmds = (Command**)calloc(32, sizeof(Command*));
            free(buffer);
            buffer = (char*)calloc(512, sizeof(char));
            j = -2;
            k = 0;
            i++;
        }
        else if(c == '='){
            flag = 1;
            j = -2;
        }
        else if(flag == 1 && j != -1 && c != '|'){
            buffer[j] = c;
        }
        else if(c == '|'){
            char* short_buff = (char*)calloc(j, sizeof(char));
            for(int l = 0; l<j-1; l++){
                short_buff[l] = buffer[l];
            }
            cmds[k] = create_command(short_buff, j-1);
            free(buffer);
            free(short_buff);
            buffer = (char*)calloc(512, sizeof(char));
            j = -2;
            k++;
        }
        j++;
    }
    free(buffer);
    free(cmds);
    fclose(f);
}

char** add_null(char** args, int len, char* program){
    char** ret = (char**)calloc(len+2, sizeof(char*));
    for(int i = 0; i<len+1; i++){
        ret[i] = (char*)calloc(128, sizeof(char));
    }
    strcpy(ret[0], program);
    for(int i = 0; i<len; i++){
        strcpy(ret[i+1], args[i]);
    }
    ret[len+1] = NULL;
    return ret;
}

int main(int argc, char* args[]){
    if(argc != 2){return 1;}
    char* file = args[1];
    int components_count = count_components(file);
    Component** arr = (Component**)calloc(components_count, sizeof(Component*));
    create_components(file, arr, components_count);
    create_commands(file, arr, components_count);

    int seq_count = count_sequences(file, components_count);
    Sequence** todo = (Sequence**)calloc(seq_count, sizeof(Sequence*));
    create_sequences(file, todo, seq_count, components_count, arr);

    for(int i = 0; i<seq_count; i++){
        printf("sequence %d out of %d:\n", i, seq_count);
        int len = 0;
        for(int j = 0; j<todo[i]->len; j++){
            len += todo[i]->components[j]->len;
        }
        int prev;
        int fds[2];

        for(int j = 0; j<todo[i]->len; j++){
            for(int l = 0; l<todo[i]->components[j]->len; l++){
                if(pipe(fds) != 0){
                    printf("pipe not created!\n");
                    return 1;
                }

                char** argz = add_null(todo[i]->components[j]->commands[l]->args, todo[i]->components[j]->commands[l]->len, todo[i]->components[j]->commands[l]->program);  
                if(fork() == 0){
                    if(!(j == todo[i]->len-1 && l == todo[i]->components[j]->len-1)){ //last cmd
                        dup2(fds[1], STDOUT_FILENO); //out
                        close(fds[1]);
                    }
                    dup2(prev, STDIN_FILENO); //in
                    close(prev);

                    execvp(todo[i]->components[j]->commands[l]->program, argz);
                    exit(1);
                }
                else{
                    close(prev);
                    close(fds[1]);
                    prev = fds[0];
                }

                for(int arg = 0; arg<todo[i]->components[j]->commands[l]->len+2; arg++){
                    free(argz[arg]);
                }
                free(argz);
            }
        }
        while(wait(NULL)>0){;}
        printf("\n");
    }

    free_memory(arr, components_count, todo, seq_count);
    return 0;
}