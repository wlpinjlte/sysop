#include<stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int wyk=2;

void licznik(int signo){
    printf("sygnal przyszedl\n");
    wyk+=1;
}

int main(int argc,char **argv){
    if(argc<3){
        printf("invalid number of arguments\n");
        return 1;
    }
    int catch_pid=atoi(argv[1]);
    int function;
    signal(SIGUSR1,licznik);
    printf("%d:\n",catch_pid);
    fflush(stdout);
    for(int i=2;i<argc;i++){
        function=atoi(argv[i]);
        sigval_t val={function};
        sigqueue(catch_pid,SIGUSR1,val);
        printf("signal send\n");
        fflush(stdout);
        pause();
        fflush(stdout);
    }
//    sleep(100);
    return 0;
}