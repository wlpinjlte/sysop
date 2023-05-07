#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <time.h>
#define BUFF_SIZE 1024
#define PATH "/tmp/integral_tmp"

int main(int argc,char **argv){
    if(argc!=3){
        printf("invalid number of arguments");
        return 1;
    }

    mkfifo(PATH,0666);

    int n_fork=atoi(argv[2]);

    time_t begin, end;
    time(&begin);

    for(int i=0;i<n_fork;i++){
        int child_pid=fork();
        if(child_pid==0){
            char ii[BUFF_SIZE];
            snprintf(ii,BUFF_SIZE,"%d",i);
            execl("./singleIntegral","singleIntegral",argv[1],argv[2],ii,NULL);
        }
    }

    int file=open(PATH,O_RDONLY);
    int counter=0;
    double result=0;

    while(counter<n_fork){
        char c[BUFF_SIZE];
        int size=read(file,c,BUFF_SIZE);
        c[size]=0;
        char *doublestr;
        doublestr=strtok(c," ");
        while(doublestr!=NULL){
            result+=strtod(doublestr,NULL);
            doublestr=strtok(NULL," ");
            counter+=1;
        }
    }
    printf("dx:%s n:%d\n",argv[1],n_fork);
    printf("result:%lf\n",result);

    time(&end);
    time_t elapsed = end - begin;
    printf("Time:%ld s\n", elapsed);
    printf("\n");
    close(file);
    return 0;
}