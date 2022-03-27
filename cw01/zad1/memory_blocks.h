#ifndef MEMORY_BLOCKS_H
#define MEMORY_BLOCKS_H

typedef struct MemoryBlock{
    char* content;
    int lines_count;
    int words_count;
    int char_count;
} MemoryBlock;

typedef struct MemoryBlockArray{
    int length;
    MemoryBlock** arr;
    int* active_arr;
} MemoryBlockArray;

MemoryBlockArray* create_memory_block_array(int n);

void delete_memory_block_array(MemoryBlockArray* memory_block_array);

MemoryBlock* create_memory_block(void);

void remove_memory_block(MemoryBlockArray* memory_block_array, int index);

int find_free_index(MemoryBlockArray* arr); //first free index

int add_memory_block(MemoryBlockArray* arr, MemoryBlock* block); //adds block to first free index, returns this index

int lc_command(MemoryBlock* block, char* file_name); //returns count

int wc_command(MemoryBlock* block, char* file_name); //returns count

int cc_command(MemoryBlock* block, char* file_name); //returns count

void read_and_copy(MemoryBlock* block, char* file_name);

void save_results(MemoryBlock* block, char* file_name);

#endif