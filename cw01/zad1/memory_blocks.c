#include "memory_blocks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MemoryBlockArray* create_memory_block_array(int n){
    MemoryBlockArray* arr = calloc(1, sizeof(MemoryBlockArray));
    arr->length = n;
    arr->arr = calloc(n, sizeof(MemoryBlock*));
    arr->active_arr = calloc(n, sizeof(int));
    for(int i = 0; i<n; i++){
        arr->active_arr[i] = 0;
    }
    return arr;
}

void delete_memory_block_array(MemoryBlockArray* memory_block_array){
    for(int i = 0; i<memory_block_array->length; i++){
        if(memory_block_array->active_arr[i] == 1){
            if(memory_block_array->arr[i]->content != NULL){
                free(memory_block_array->arr[i]->content);
            }
            free(memory_block_array->arr[i]);
        }
    }
    free(memory_block_array->active_arr);
    free(memory_block_array->arr);
    free(memory_block_array);
}

MemoryBlock* create_memory_block(void){
    MemoryBlock* block = calloc(1, sizeof(MemoryBlock));
    block->lines_count = -1;
    block->words_count = -1;
    block->char_count = -1;
    block->content = NULL;
    return block;
}

void remove_memory_block(MemoryBlockArray* memory_block_array, int index){
    if(memory_block_array->active_arr[index] == 1){
        if(memory_block_array->arr[index]->content != NULL){
            free(memory_block_array->arr[index]->content);
        }
        free(memory_block_array->arr[index]);
    }
    memory_block_array->active_arr[index] = 0;
}

int find_free_index(MemoryBlockArray* arr){
    for(int i = 0; i < arr->length; i++){
        if(arr->active_arr[i] == 0){
            return i;
        }
    }
    return -1; //not found
}

int add_memory_block(MemoryBlockArray* arr, MemoryBlock* block){
    int index = find_free_index(arr);
    if(index == -1){
        printf("%s\n", "no free indexes!\n");
        return -1;
    }
    arr->active_arr[index] = 1;
    arr->arr[index] = block;
    return index;
}

int lc_command(MemoryBlock* block, char* file_name){
    char command[64] = "wc -l ";
    strcat(command, file_name);
    FILE* f = popen(command, "r");
    char c;
    char res[16] = "";
    int ret;
    int i = 0;
    while((c = fgetc(f)) != '.'){
        res[i] = c;
        i++;
    }
    ret = 1 + atoi(res);
    fclose(f);
    block->lines_count = ret;
    return ret;
}

int wc_command(MemoryBlock* block, char* file_name){
    char command[64] = "wc -w ";
    strcat(command, file_name);
    FILE* f = popen(command, "r");
    char c;
    char res[16] = "";
    int ret;
    int i = 0;
    while((c = fgetc(f)) != '.'){
        res[i] = c;
        i++;
    }
    ret = atoi(res);
    fclose(f);
    block->words_count = ret;
    return ret;
}

int cc_command(MemoryBlock* block, char* file_name){
    char command[64] = "wc -c ";
    strcat(command, file_name);
    FILE* f = popen(command, "r");
    char c;
    char res[16] = "";
    int ret;
    int i = 0;
    while((c = fgetc(f)) != '.'){
        res[i] = c;
        i++;
    }
    ret = atoi(res);
    fclose(f);
    block->char_count = ret;
    return ret;
}

void read_and_copy(MemoryBlock* block, char* file_name){
    FILE* f = fopen(file_name, "r");
    char c;
    int len = cc_command(block, file_name);
    block->content = calloc(len+1, sizeof(char));
    int i = 0;
    while((c = fgetc(f)) != EOF){
        block->content[i] = c;
        i++;
    }
    fclose(f);
}

void save_results(MemoryBlock* block, char* file_name){
    FILE* f = fopen("./results_of_counting_if_enabled.txt", "w");
    char lines[32] = "lines: ";
    char words[32] = "words: ";
    char chars[32] = "characters: ";
    char lc[16];
    char wc[16];
    char cc[16];
    sprintf(lc, "%d", block->lines_count);
    sprintf(wc, "%d", block->words_count);
    sprintf(cc, "%d", block->char_count);
    strcat(lines, lc);
    strcat(words, wc);
    strcat(chars, cc);
    fputs(file_name, f);
    fputc('\n', f);
    fputs(lines, f);
    fputc('\n', f);
    fputs(words, f);
    fputc('\n', f);
    fputs(chars, f);
    fputc('\n', f);
    fclose(f);
}