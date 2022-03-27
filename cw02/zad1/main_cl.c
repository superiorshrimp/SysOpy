#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>
#include <string.h>

int main(int argc, char* args[]){
    struct timespec r_start;
    struct timespec r_end;
    struct tms st_time;
    struct tms end_time;
    times(&st_time);
    clock_gettime(CLOCK_REALTIME, &r_start);
    FILE* src;
    FILE* dest;
    if(argc == 1){
        char src_name[128];
        char dest_name[128];
        printf("source file name: ");
        scanf("%s", src_name);
        printf("destination file name: ");
        scanf("%s", dest_name);
        src = fopen(src_name, "r");
        dest = fopen(dest_name, "w");
    }
    else if(argc == 3){
        src = fopen(args[1], "r");
        dest = fopen(args[2], "w");
    }
    else{return 1;}
    int f = 0;
    int i = 1;
    int flag = 0;
    char c = fgetc(src);
    while(c != EOF){
        if(c == '\n'){
            if(flag == 1){
                if(f == 1){
                    fputc('\n', dest);
                }
                else{
                    f = 1;
                }
                fseek(src, -i, 1);
                c = fgetc(src);
                while(c != '\n'){
                    fputc(c, dest);
                    c = fgetc(src);
                }
            }
            i = 0;
            flag = 0;
        }
        else if(!isspace(c) && c != ' '){
            flag = 1;
        }
        c = fgetc(src);
        i++;
    }
    fseek(src, -i + 1, 1);
    c = fgetc(src);
    while(c != EOF && c != '\n' && flag == 1){
        fputc(c, dest);
        c = fgetc(src);
    }

    fclose(src);
    fclose(dest);

    clock_gettime(CLOCK_REALTIME, &r_end);
    times(&end_time);
    printf("%s %f\n", "real time: ", (double) ((r_end.tv_sec-r_start.tv_sec) + (r_end.tv_nsec-r_start.tv_nsec)/1000000000.0));
    printf("%s %f\n", "user time: ", ((double) end_time.tms_utime - st_time.tms_utime)/sysconf(_SC_CLK_TCK));
    printf("%s %f\n", "system time: ", ((double) end_time.tms_stime - st_time.tms_stime)/sysconf(_SC_CLK_TCK));
    FILE* f1 = fopen("./pomiar_zad_1_cl.txt", "a");
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