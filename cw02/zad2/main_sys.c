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
    if(argc != 3){printf("wrong count of arguments!"); return 1;}
    char c = *args[1];
    int src = open(args[2], O_RDONLY,S_IRUSR|S_IWUSR);

    const int len = 257;
    char ch[len];
    int run = read(src, ch, len - 1);
    int count = 0;
    int lines_count = 0;
    int flag = 0;
    while(run > 0){
        for(int i = 0; i<run; i++){
            if(ch[i] == c){
                count++;
                if(flag == 0){
                    lines_count++;
                    flag = 1;
                }
            }
            if(ch[i] == '\n'){
                flag = 0;
            }
        }
        run = read(src, ch, len - 1);
    }
    close(src);
    printf("%c was found %d times in total, in %d different lines\n", c, count, lines_count);

    clock_gettime(CLOCK_REALTIME, &r_end);
    times(&end_time);
    printf("%s %f\n", "real time: ", (double) ((r_end.tv_sec-r_start.tv_sec) + (r_end.tv_nsec-r_start.tv_nsec)/1000000000.0));
    printf("%s %f\n", "user time: ", ((double) end_time.tms_utime - st_time.tms_utime)/sysconf(_SC_CLK_TCK));
    printf("%s %f\n", "system time: ", ((double) end_time.tms_stime - st_time.tms_stime)/sysconf(_SC_CLK_TCK));
    FILE* f1 = fopen("./pomiar_zad_2_sys.txt", "a");
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