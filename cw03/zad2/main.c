#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/times.h>

//integral from 0 to 1 of 4/(x^2+1) dx

double min(double a, double b){
    if(a < b){return a;}
    else{return b;}
}

double f(double x){return 4/(x*x+1);}

double calculate(double a, double b){return (b - a) * f(b);}

void save(double val, int i){
    char* name = (char*)calloc(2, sizeof(char));
    char* number = (char*)calloc(16, sizeof(char));
    char* extension = (char*)calloc(5, sizeof(char));
    strcat(name, "w");
    strcat(extension, ".txt");
    sprintf(number, "%d", i);
    strcat(name, number);
    strcat(name, extension);
    FILE* f = fopen(name, "w");
    char* result = (char*)calloc(33, sizeof(char));
    sprintf(result, "%f", val);
    fputs(result, f);
    fclose(f);
    free(name);
    free(number);
    free(extension);
}

int main(int argc, char* args[]){
    if(argc != 3){return 1;}
    struct timespec r_start;
    struct timespec r_end;
    struct tms st_time;
    struct tms end_time;
    times(&st_time);
    clock_gettime(CLOCK_REALTIME, &r_start);
    double w = atof(args[1]); //width of a rectangle
    int n = atoi(args[2]); //number of processes
    int d = 0; //number of intervals
    double a = 0.0;
    double b = 1.0;
    while(a < b){
        a += w;
        d++;
    }
    double* todo = (double*)calloc(d, sizeof(double));
    int i = 0;
    a = 0.0;
    while(a < b){
        todo[i] = a;
        a += w;
        i++;
    }
    for(int i = 0; i<n; i++){
        pid_t child_pid = fork();
        if(child_pid == 0){ //child
            double sum = 0;
            for(int j = 0; j<1+(int)(d/n); j++){
                if(i + j*n<d){
                    sum += calculate(todo[i + j*n], min(w + todo[i + j*n], 1.0));
                }
                else{break;}
            }
            save(sum, 1 + i);
            exit(0);
        }
    }
    while(wait(NULL) > 0);
    double s = 0.0;
    for(int i = 1; i<=n; i++){
        char* name = (char*)calloc(2, sizeof(char));
        char* number = (char*)calloc(16, sizeof(char));
        char* extension = (char*)calloc(5, sizeof(char));
        strcat(name, "w");
        strcat(extension, ".txt");
        sprintf(number, "%d", i);
        strcat(name, number);
        strcat(name, extension);
        FILE* f = fopen(name, "r");
        char* part_res = (char*)calloc(9, sizeof(char));
        for(int i = 0; i<8; i++){ 
            part_res[i] = fgetc(f);
        }
        s += atof(part_res);
        free(part_res);
        fclose(f);
    }
    printf("result: %f\n", s);

    clock_gettime(CLOCK_REALTIME, &r_end);
    times(&end_time);
    printf("%s %f\n", "real time: ", (double) ((r_end.tv_sec-r_start.tv_sec) + (r_end.tv_nsec-r_start.tv_nsec)/1000000000.0));
    printf("%s %f\n", "user time: ", ((double) end_time.tms_utime - st_time.tms_utime)/sysconf(_SC_CLK_TCK));
    printf("%s %f\n", "system time: ", ((double) end_time.tms_stime - st_time.tms_stime)/sysconf(_SC_CLK_TCK));
    return 0;
}