#include "header.h"


int queueKey[MAX_NO_CLIENTS];
key_t serverId;


int getAvailableIndex(){
    for(int i=0;i<MAX_NO_CLIENTS;i++){
        if(queueKey[i]==-1){
            return i;
        }
    }
    return -1;
}

int getIndex(int key){
    for(int i=0;i<MAX_NO_CLIENTS;i++){
        if(queueKey[i]==key){
            return i;
        }
    }
    return -1;
}

void init(struct msgbuff *buff){
    int index=getAvailableIndex();
    if(index==-1){
        strcpy(buff->mtext,"too much clients");
    }else{
        queueKey[index]=buff->clientKey;
        buff->auxiliaryId=index;
    }
    //printf("%d \n",buff->clientKey);
    //fflush(NULL);
    msgsnd(msgget(buff->clientKey,0),buff,sizeof(struct msgbuff),0);
    //printf("send\n");
}

void LIST(struct msgbuff *buff){
    strcpy(buff->mtext,"");
    for(int i=0;i<MAX_NO_CLIENTS;i++){
        if(queueKey[i]!=-1){
            sprintf(buff->mtext,"%d ",i);
        }
    }
    msgsnd(msgget(buff->clientKey,0),buff,sizeof(struct msgbuff),0);
}

void ALL(struct msgbuff *buff){
    for(int i=0;i<MAX_NO_CLIENTS;i++){
        if(queueKey[i]!=-1&&queueKey[i]!=buff->clientKey){
            msgsnd(msgget(queueKey[i],0),buff,sizeof(struct msgbuff),0);
        }
    }
}

void ONE(struct msgbuff *buff){
    msgsnd(msgget(queueKey[buff->auxiliaryId],0),buff,sizeof(struct msgbuff),0);
}

void STOP(struct msgbuff *buff){
    int index=getIndex(buff->clientKey);
    if(index!=-1){
        queueKey[index]=-1;
    }
}

void handler(int signum){
    //struct msgbuff *buff;

    msgctl(serverId, IPC_RMID, NULL);
    exit(0);
}

int main(){
    signal(SIGINT,handler);
    for(int i=0;i<MAX_NO_CLIENTS;i++){
        queueKey[i]=-1;
    }
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    serverId=msgget(ftok(HOME,1),IPC_CREAT | 0666);
    while(1) {
        msgrcv(serverId, buff, sizeof(struct msgbuff), -10, 0);
        printf("dostal\n");
        if (buff->mtype == 1) {
            init(buff);
        } else if (buff->mtype == 2) {
            LIST(buff);
        } else if (buff->mtype == 3) {
            ALL(buff);
        } else if (buff->mtype == 4) {
            ONE(buff);
        } else if (buff->mtype == 5) {
            STOP(buff);
        } else {
            printf("unknow type");
        }
    }
}