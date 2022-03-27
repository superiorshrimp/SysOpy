#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../zad1/memory_blocks.h"
#include <time.h>
#include <unistd.h>
#include <sys/times.h>

int main(int argc, char* args[]){
    //args[1] - length of main array
    //scheme (initially i = 2, example below):
    //args[i] - command
    //args[i+1] - how many arguments to that command
    //args[i+2..i+2+args[i+1]]
    //repeat scheme for next commands
    //scheme in other words: what to do (command) -> how many arguments -> arguments 
    //commands:
    //0 - count lines, words, characters
    //1 - read and copy file(s)
    //2 - 0 and 1
    //3 - remove memory block with index(es)
    //4 - count lines
    //5 - count words
    //6 - count characters
    //7 - 0 with save to file results_of_counting_if_enabled.txt
    //8 - 2 with save to file results_of_counting_if_enabled.txt
    //for example arguments: 4 0 4 ./data/sf1.txt ./data/sf2.txt ./data/sf3.txt ./data/sf4.txt 3 1 1 2 1 ./data/sf5.txt 3 2 0 2 8 2 ./data6/sf.txt ./data/sf7.txt
    //1: it will create an array of length 4 (for memory blocks)
    //2: it will count lines, words and characters of 4 files (./data/sf1.txt, ./data/sf2.txt, ./data/sf3.txt, ./data/sf4.txt)
    //3: it will remove 1 memory block from array (of index 1)
    //4: it will read and copy + count l,w,c of 1 file (./data/sf5.txt)
    //5: it will remove 2 memory blocks from array (indexes: 0 and 2)
    //6: it will read and copy + count l,w,c and save results of counting to file results_of_counting_if_enabled.txt of 2 files (./data/sf6.txt and ./data/sf7.txt)
    if(argc<2){printf("%s\n", "too few arguments! exiting..."); return 0;}
    FILE* f0 = fopen("./results_of_counting_if_enabled.txt", "w"); //clearing whatever was there before
    fclose(f0);
    struct timespec r_start;
    struct timespec r_end;
    struct tms st_time;
    struct tms end_time;
    times(&st_time);
    clock_gettime(CLOCK_REALTIME, &r_start);
    MemoryBlockArray* arr = create_memory_block_array(atoi(args[1]));
    int i = 2;
    while(i < argc){
        if(atoi(args[i]) == 0){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                MemoryBlock* block = create_memory_block();
                add_memory_block(arr, block);
                lc_command(block, args[i+2+j]);
                wc_command(block, args[i+2+j]);
                cc_command(block, args[i+2+j]);
            }
            i += 2 + atoi(args[i + 1]);
        }
        else if(atoi(args[i]) == 1){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                MemoryBlock* block = create_memory_block();
                add_memory_block(arr, block);
                read_and_copy(block, args[i+2+j]);
            }
            i += 2 + atoi(args[i + 1]);
        }
        else if(atoi(args[i]) == 2){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                MemoryBlock* block = create_memory_block();
                add_memory_block(arr, block);
                read_and_copy(block, args[i+2+j]);
                lc_command(block, args[i + 2 + j]);
                wc_command(block, args[i + 2 + j]);
            }
            i += 2 + atoi(args[i + 1]);
        }
        else if(atoi(args[i]) == 3){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                remove_memory_block(arr, atoi(args[i + 2 + j]));
            }
            i += 2 + atoi(args[i + 1]);
        }
        else if(atoi(args[i]) == 4){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                MemoryBlock* block = create_memory_block();
                add_memory_block(arr, block);
                lc_command(block, args[i+2+j]);
            }
            i += 2 + atoi(args[i + 1]);
        }
        else if(atoi(args[i]) == 5){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                MemoryBlock* block = create_memory_block();
                add_memory_block(arr, block);
                wc_command(block, args[i+2+j]);
            }
            i += 2 + atoi(args[i + 1]);
        }
        else if(atoi(args[i]) == 6){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                MemoryBlock* block = create_memory_block();
                add_memory_block(arr, block);
                cc_command(block, args[i+2+j]);
            }
            i += 2 + atoi(args[i + 1]);
        }
        else if(atoi(args[i]) == 7){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                MemoryBlock* block = create_memory_block();
                add_memory_block(arr, block);
                lc_command(block, args[i+2+j]);
                wc_command(block, args[i+2+j]);
                cc_command(block, args[i+2+j]);
                save_results(block, args[i + 2 + j]);
            }
            i += 2 + atoi(args[i + 1]);
        }
        else if(atoi(args[i]) == 8){
            for(int j = 0; j < atoi(args[i + 1]); j++){
                MemoryBlock* block = create_memory_block();
                add_memory_block(arr, block);
                read_and_copy(block, args[i+2+j]);
                lc_command(block, args[i + 2 + j]);
                wc_command(block, args[i + 2 + j]);
                save_results(block, args[i + 2 + j]);
            }
            i += 2 + atoi(args[i + 1]);
        }
        else{
            printf("%s\n", "incorrect arguments passed! exiting...");
            return 1;
        }
    }
    delete_memory_block_array(arr);
    clock_gettime(CLOCK_REALTIME, &r_end);
    times(&end_time);
    printf("%s %f\n", "real time: ", (double) ((r_end.tv_sec-r_start.tv_sec) + (r_end.tv_nsec-r_start.tv_nsec)/1000000000.0));
    printf("%s %f\n", "user time: ", ((double) end_time.tms_utime - st_time.tms_utime)/sysconf(_SC_CLK_TCK));
    printf("%s %f\n", "system time: ", ((double) end_time.tms_stime - st_time.tms_stime)/sysconf(_SC_CLK_TCK));
    FILE* f1 = fopen("./results.txt", "a");
    char real[32] = "real time: ";
    char user[32] = "user time: ";
    char system[32] = "system time: ";
    char r[32];
    char u[32];
    char s[32];
    sprintf(r, "%f", (double) ((r_end.tv_sec-r_start.tv_sec) + (r_end.tv_nsec-r_start.tv_nsec)/1000000000.0));
    sprintf(u, "%f", ((double) end_time.tms_utime - st_time.tms_utime)/sysconf(_SC_CLK_TCK));
    sprintf(s, "%f", ((double) end_time.tms_stime - st_time.tms_stime)/sysconf(_SC_CLK_TCK));
    strcat(real, r);
    strcat(user, u);
    strcat(system, s);
    fputs(real, f1);
    fputs("\n", f1);
    fputs(user, f1);
    fputs("\n", f1);
    fputs(system, f1);
    fputs("\n", f1);
    fclose(f1);
    return 0;
}