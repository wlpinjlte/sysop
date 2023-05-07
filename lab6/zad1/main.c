#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include <stdlib.h>
#include <string.h>
struct msgbuff {
    long mtype;         /* typ komunikatu   */
    char mtext[1024];      /* tresc komunikatu */
};
int main(){
    //printf("siema10");
    //fflush(NULL);
    int key=ftok("/home/waga/sysop/lab6/zad1",0);
    int id=msgget( key,IPC_CREAT | 0666);
    struct msgbuff *buff;
    //printf("siema1");
    buff=malloc(sizeof(struct msgbuff));
    //printf("siema2");
    buff->mtype=10;
    strcpy(buff->mtext,"siema");
    msgsnd(id,buff,sizeof(buff),0);
    buff=calloc(1,sizeof(struct msgbuff));
    msgrcv(id,buff,sizeof(buff->mtext),10,0);
    printf("%s",buff->mtext);
    fflush(NULL);
    //msgrcv(id,buff,sizeof(buff->mtext),-10,0);
    return 0;
}