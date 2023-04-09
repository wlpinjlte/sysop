#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include <sys/times.h>
#include <dlfcn.h>
#define BUFF_SIZE 1024

double f(double x){
    return 4/(x*x+1);
}

double integral(double dx,int i,double long_of_reactagle){
    double result=0;
    double j=i*long_of_reactagle;
    double end=long_of_reactagle*(i+1);
    double steps=long_of_reactagle/dx;
    for(int g=0;g<steps-1;g++){
        result+=f(j)*dx;
        j+=dx;
    }
    result+=f(j)*(end-j);
    printf("%lf\n",result);
    printf("\n");
    return result;
}

double timeDifference(clock_t t1, clock_t t2){
    return ((double)(t2 - t1) / sysconf(_SC_CLK_TCK));
}

void writeResult(clock_t start, clock_t end,struct tms t_start,struct tms t_end){
    printf("\tREAL_TIME: %fl\n", timeDifference(start,end));
    printf("\tUSER_TIME: %fl\n", timeDifference(t_start.tms_utime, t_end.tms_utime));
    printf("\tSYSTEM_TIME: %fl\n", timeDifference(t_start.tms_stime, t_end.tms_stime));
}

int main(int argc,char **argv){
    if(argc!=3){
        printf("invalid number of arguments");
        return 1;
    }
    double dx=strtod(argv[1],NULL);
    int n_fork=atoi(argv[2]);
    double length_of_reactagle=1.0/n_fork;
    int fd[2];
    double result=0;

    clock_t timeStart,timeEnd;
    static struct tms tmsStart,tmsEnd;
    timeStart=0;
    timeEnd=0;
    timeStart=times(&tmsStart);
    for(int i=0;i<n_fork;i++){
        pipe(fd);
        int child_pid=fork();
        if(child_pid==0){
            close(fd[0]);
            char c[BUFF_SIZE];
            snprintf(c,BUFF_SIZE,"%lf",integral(dx,i,length_of_reactagle));
            write(fd[1],c,strlen(c));
            exit(0);
        }else{
            char c[BUFF_SIZE];
            close(fd[1]);
            wait(NULL);
            int size=read(fd[0],c,BUFF_SIZE);
            c[size]=0;
            printf("%s\n",c);
            result+=strtod(c,NULL);
        }
    }
    printf("%lf\n",result);
    timeEnd=times(&tmsEnd);
    writeResult(timeStart, timeEnd,
                tmsStart, tmsEnd);
    return 0;
}