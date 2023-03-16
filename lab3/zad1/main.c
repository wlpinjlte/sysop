#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>
#include <stdlib.h>
int main(int argc,char **argv){
    if(argc!=2){
        printf("zla liczba argumentow\n");
        return 1;
    }
    int child_pid;
    int i=0;
    int n=atoi(argv[1]);
    int main_pid=(int)getpid();
    for(;i<n;i++){
        child_pid=-1;
        if((int)getpid()==main_pid){
            child_pid=fork();
        }
        if(child_pid==0){
            printf("Proces rodzica ma pid:%d\n",(int)getppid());
            printf("Proces dziecka ma pid:%d\n",(int)getpid());
        }
    }
    while (wait(NULL) > 0);
    if((int)getpid()==main_pid){
        printf("Proces macierzysty argv[1]:%s\n",argv[1]);
    }
    return 0;
}