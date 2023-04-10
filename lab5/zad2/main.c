#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#define BUFF_SIZE 1024

double f(double x){
    return 4/(x*x+1);
}

double integral(double dx,int i,double long_of_reactagle){
    double result=0;
    double j=i*long_of_reactagle;
    double end=long_of_reactagle*(i+1);
    long steps=long_of_reactagle/dx;
    for(long g=0;g<steps-1;g++){
        result+=f(j)*dx;
        j+=dx;
    }
    result+=f(j)*(end-j);
//    printf("%lf\n",result);
//    printf("\n");
    return result;
}

int main(int argc,char **argv){
    if(argc!=3){
        printf("invalid number of arguments");
        return 1;
    }
    double dx=strtod(argv[1],NULL);
    int n_fork=atoi(argv[2]);
    double length_of_reactagle=1.0/n_fork;
    int fd[n_fork][2];
    double result=0;

    time_t begin, end;
    time(&begin);

    for(int i=0;i<n_fork;i++){
        pipe(fd[i]);
        int child_pid=fork();
        if(child_pid==0){
            close(fd[i][0]);
            char c[BUFF_SIZE];
            snprintf(c,BUFF_SIZE,"%lf",integral(dx,i,length_of_reactagle));
            write(fd[i][1],c,strlen(c));
            exit(0);
        }else{
            close(fd[i][1]);
        }
    }
    while(wait(NULL)>0)
    for(int i=0;i<n_fork;i++){
        char c[BUFF_SIZE];
        int size=read(fd[i][0],c,BUFF_SIZE);
        c[size]=0;
//        printf("%s\n",c);
        result+=strtod(c,NULL);
    }
    printf("dx:%s n:%d\n",argv[1],n_fork);
    printf("Result: %lf\n",result);

    time(&end);
    time_t elapsed = end - begin;
    printf("Time:%ld s\n", elapsed);
    printf("\n");
    return 0;
}