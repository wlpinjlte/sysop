#include<stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
void obslugaUrs1(int signum){
    printf("pid:%d sygnal\n",getpid());
}

void pending_function(){
    sigset_t sigset;
    sigpending(&sigset);
    if(sigismember(&sigset,SIGUSR1)){
        printf("pid:%d signal pending\n",getpid());
    }else{
        printf("pid:%d signal not pending\n",getpid());
    }
}

void block_function(){
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGUSR1);
    sigprocmask(SIG_BLOCK,&sigset,NULL);
}

void raise_function(){
    printf("pid:%d signal rised\n",getpid());
    raise(SIGUSR1);
}

int function(char **argv){
    if(strcmp(argv[1],"ignore")==0){
        signal(SIGUSR1,SIG_IGN);
        raise_function();
        return 1;
    }else if(strcmp(argv[1],"handler")==0){
        signal(SIGUSR1, obslugaUrs1);
        raise_function();
        return 1;
    }else if(strcmp(argv[1],"mask")==0){
        signal(SIGUSR1, obslugaUrs1);
        if(strcmp(argv[2],"parent")==0){
            block_function();
            raise_function();
        }
        return 1;
    }else if(strcmp(argv[1],"pending")==0){
        signal(SIGUSR1, obslugaUrs1);
        if(strcmp(argv[2],"parent")==0){
            block_function();
            raise_function();
        }
        pending_function();
        return 1;
    }else{
        printf("invalid command");
        return 0;
    }
}

int main(int argc,char **argv){
    if(argc!=4){
        printf("invalide number of argument");
        return 0;
    }

    if(function(argv)==0){
        return 0;
    }

    if(strcmp(argv[3],"execl")==0 && strcmp(argv[2],"parent")==0){
        execl(argv[0],argv[0],argv[1],"child","execl",NULL);
    }
    if(strcmp(argv[3],"fork")==0 && strcmp(argv[2],"parent")==0){
        int child_pid=fork();
        if(child_pid==0){
            if(strcmp(argv[1],"pending")==0){
                pending_function();
            }else{
                raise_function();
            }
        }
    }
    return 0;
}