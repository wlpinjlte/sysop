#include "header.h"


int queueKey[MAX_NO_CLIENTS];
key_t serverId;
FILE *logs;

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
    fprintf(logs, "%02d:%02d:%02d INIT Client ID: ", buff->timeinfo.tm_hour, buff->timeinfo.tm_min, buff->timeinfo.tm_sec);
    if(index==-1){
        strcpy(buff->mtext,"too much clients");
        fprintf(logs,"limit of clients reached\n");
    }else{
        queueKey[index]=buff->clientKey;
        buff->auxiliaryId=index;
        fprintf(logs,"%d\n",index);
    }
    msgsnd(msgget(buff->clientKey,0),buff,sizeof(struct msgbuff),0);
}

void LIST(struct msgbuff *buff){
    fprintf(logs,"%02d:%02d:%02d LIST Client ID: %d\n",buff->timeinfo.tm_hour,buff->timeinfo.tm_min,buff->timeinfo.tm_sec,
            getIndex(buff->clientKey));
    strcpy(buff->mtext,"");
    for(int i=0;i<MAX_NO_CLIENTS;i++){
        if(queueKey[i]!=-1){
            sprintf(buff->mtext+strlen(buff->mtext),"%d ",i);
        }
    }
    msgsnd(msgget(buff->clientKey,0),buff,sizeof(struct msgbuff),0);
}

void ALL(struct msgbuff *buff){
    fprintf(logs,"%02d:%02d:%02d 2ALL Client ID:%d TEXT:%s\n",buff->timeinfo.tm_hour,buff->timeinfo.tm_min,buff->timeinfo.tm_sec,
            getIndex(buff->clientKey),buff->mtext);
    buff->auxiliaryId= getIndex(buff->clientKey);
    for(int i=0;i<MAX_NO_CLIENTS;i++){
        if(queueKey[i]!=-1&&queueKey[i]!=buff->clientKey){
            msgsnd(msgget(queueKey[i],0),buff,sizeof(struct msgbuff),0);
        }
    }
}

void ONE(struct msgbuff *buff){
    fprintf(logs,"%02d:%02d:%02d 2ONE Client ID:%d TO:%d TEXT:%s\n",buff->timeinfo.tm_hour,buff->timeinfo.tm_min,buff->timeinfo.tm_sec,
            getIndex(buff->clientKey),buff->auxiliaryId,buff->mtext);
    int auxiliary=buff->auxiliaryId;
    buff->auxiliaryId= getIndex(buff->clientKey);
    msgsnd(msgget(queueKey[auxiliary],0),buff,sizeof(struct msgbuff),0);
}

void STOP(struct msgbuff *buff){
    fprintf(logs,"%02d:%02d:%02d STOP Client ID:%d\n",buff->timeinfo.tm_hour,buff->timeinfo.tm_min,buff->timeinfo.tm_sec,
            getIndex(buff->clientKey));
    int index=getIndex(buff->clientKey);
    if(index!=-1){
        queueKey[index]=-1;
    }
}

void handler(int signum){
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    buff->mtype=5;
    for(int i=0;i<MAX_NO_CLIENTS;i++){
        if(queueKey[i]!=-1){
            msgsnd(msgget(queueKey[i],0),buff,sizeof(struct msgbuff),0);
            msgrcv(serverId,buff,sizeof(struct msgbuff),5,0);
        }
    }
    free(buff);
    fclose(logs);
    msgctl(serverId, IPC_RMID, NULL);
    exit(0);
}

int main(){
    logs=fopen("log.txt","a");
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
            printf("unknow type\n");
        }
    }
}