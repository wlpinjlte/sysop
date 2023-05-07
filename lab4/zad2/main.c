#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
extern int errno;
void info(int signo, siginfo_t* siginfo, void* context){
    printf("singal: %d\n",siginfo->si_signo);
    printf("PID: %d\n",siginfo->si_pid);
    printf("exit value or signal: %x\n",siginfo->si_status);
    printf("Number of attempted system call: %d\n",siginfo->si_syscall);
    printf("Real user ID: %d\n",siginfo->si_uid);
}

void child(int signo, siginfo_t* siginfo, void* context){
    printf("child terminated\n");
}

void test2(struct sigaction act){
//    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction=child;
    act.sa_flags=SA_NOCLDSTOP;
    sigaction(SIGCHLD,&act,NULL);
    int child_pid=fork();
    if(child_pid!=0) {
        kill(child_pid, SIGSTOP);
    }else{
        wait(NULL);
    }
}

void test3(struct sigaction act){
//    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction=child;
    act.sa_flags=SA_NODEFER;
    sigaction(SIGCHLD,&act,NULL);
    int child_pid=fork();
    if(child_pid!=0) {
        kill(child_pid, SIGSTOP);
        exit(0);
    }else{
        wait(NULL);
    }
    pause();
}

void test1(struct sigaction act){
//    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction=info;
    act.sa_flags=SA_SIGINFO;
    sigaction(SIGUSR1,&act,NULL);

    printf("self:\n");
    kill(getpid(),SIGUSR1);

    printf("\n");

    printf("child:\n");
    int child_pid=fork();
    if(child_pid==0) {
        kill(getpid(), SIGUSR1);
        exit(0);
    }else{
        wait(NULL);
    }
    printf("\n");
    printf("Custom:\n");

    sigval_t val={10};
    sigqueue(getpid(),SIGUSR1,val);
}

int main(){
//    printf("%d\n",temp);
    struct sigaction act;
    printf("test1: \n");
    test1(act);
    wait(NULL);
    printf("\n");
    printf("test2: \n");
    test2(act);
    printf("test3: \n");
    test3(act);
    wait(NULL);
    return 0;
}