#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>

int main(int argc, char* args[]){
    struct timespec r_start;
    struct timespec r_end;
    struct tms st_time;
    struct tms end_time;
    times(&st_time);
    clock_gettime(CLOCK_REALTIME, &r_start);
    int src;
    int dest;
    if(argc == 1){
        char src_name[128];
        char dest_name[128];
        printf("source file name: ");
        scanf("%s", src_name);
        printf("destination file name: ");
        scanf("%s", dest_name);
        src = open(src_name, O_RDONLY);
        dest = open(dest_name, O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    }
    else if(argc == 3){
        src = open(args[1], O_RDONLY);
        dest = open(args[2], O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    }
    else{return 1;}
    const int len = 257; //max line length is 256
    char c[len];
    int flag = 0;
    int run = read(src, c, len - 1);
    int last = 0;
    int f = 0;
    while(run > 0){
        for(int j = 0; j<run; j++){
            if(c[j] == '\n'){
                if(flag == 1){
                    if(f == 1){
                    write(dest, "\n",1);
                    }
                    else{
                        f = 1;
                    }
                    char* rewriter = (char*)calloc(j+1, sizeof(char));
                    for(int i = 0; i<j+1; i++){
                        rewriter[i] = c[i];
                    }
                    write(dest, rewriter, j);
                    free(rewriter);
                }
                lseek(src, -run + j + 1 , SEEK_CUR);
                flag = 0;
                last = 0;
                break;
            }
            else if(c[j] != ' ' && !isspace(c[j])){
                flag = 1;
            }
            last++;
        }
        run = read(src, c, len - 1);
    }
    if(last != 0){
        char* rewriter = (char*)calloc(last, sizeof(char));
        for(int i = 0; i<last; i++){
            rewriter[i] = c[i];
            if(c[i] == '\n'){
                last--;
            }
        }
        
        write(dest, rewriter, last);
        free(rewriter);
    }
    
    

    close(src);
    close(dest);
    clock_gettime(CLOCK_REALTIME, &r_end);
    times(&end_time);
    printf("%s %f\n", "real time: ", (double) ((r_end.tv_sec-r_start.tv_sec) + (r_end.tv_nsec-r_start.tv_nsec)/1000000000.0));
    printf("%s %f\n", "user time: ", ((double) end_time.tms_utime - st_time.tms_utime)/sysconf(_SC_CLK_TCK));
    printf("%s %f\n", "system time: ", ((double) end_time.tms_stime - st_time.tms_stime)/sysconf(_SC_CLK_TCK));
    FILE* f1 = fopen("./pomiar_zad_1_sys.txt", "a");
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
    fputc('\n', f1);
    fputs(user, f1);
    fputc('\n', f1);
    fputs(system, f1);
    fputc('\n', f1);
    fclose(f1);

    return 0;
}