#include<stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int wyk=2;

void licznik(int signo){
    wyk+=1;
}

int main(int argc,char **argv){
    if(argc!=3){
        printf("invalid number of arguments");
        return 1;
    }
    int catch_pid=atoi(argv[1]);
    int function;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGUSR1);
    for(int i=2;i<argc;i++){
        function=atoi(argv[i]);

        signal(SIGUSR1,licznik);

        sigval_t val={function};
        sigqueue(catch_pid,SIGUSR1,val);

        sigsuspend(set);
    }
    return 0;
}