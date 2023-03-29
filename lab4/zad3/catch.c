#include<stdio.h>
#include<signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int parameter=0;
int status=0;
int counter=0;

void handler(int signo,siginfo_t* siginfo,void * context){
    printf("sygnal przyszedl:%d\n",siginfo->si_pid);
    status=1;
    parameter=siginfo->si_status;
    kill(siginfo->si_pid,SIGUSR1);
    counter+=1;
}

void printCounter(){
//    fflush(stdout);
    printf("counter:%d\n",counter);
}

void end(){
    printf("end");
    exit(0);
}

void for1to100(){
    for(int i=1;i<=100;i++){
//        fflush(stdout);
        printf("%d ",i);
    }
    printf("\n");
    exit(0);
}

void printTime(){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
//    fflush(stdout);
    printf ( "Current local time and date: %s", asctime (timeinfo) );
}

void printEverySecond(){
    while(parameter==4){
//        fflush(stdout);
        printTime();
        sleep(1);
    }
}

int main(){
    printf("pid: %d\n",getpid());
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction=handler;
    act.sa_flags=SA_SIGINFO;
    sigaction(SIGUSR1,&act,NULL);
    while(1){
        if(status==0)
            continue;
        status=0;
        if(parameter==1){
            if(fork()==0){
//                printf("paramter1\n");
                signal(SIGUSR1,SIG_IGN);
                for1to100();
                fflush(stdout);
            }
        }else if(parameter==2){
            if(fork()==0){
//                printf("parametr2\n");
                signal(SIGUSR1,SIG_IGN);
                printTime();
                fflush(stdout);
                exit(0);
            }

        }else if(parameter==3){
            fflush(stdout);
            if(fork()==0){
//                printf("parametr3\n");
                signal(SIGUSR1,SIG_IGN);
                printCounter();
                fflush(stdout);
                exit(0);
            }

        }else if(parameter==4){
//            printf("parametr4\n");
            printEverySecond();
            fflush(stdout);
        }else if(parameter==5){
//            printf("parametr5\n");
            printf("end");
            fflush(stdout);
            end();
        }
//        fflush(stdout);

    }


    return 0;
}