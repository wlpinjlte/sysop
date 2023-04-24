#include "header.h"
key_t key;
int queueID;
void init(int serverId){
    time_t time1;
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    time(&time1);
    buff->timeinfo=localtime(&time1);
    printf("%d\n",key);
    buff->clientKey=key;
    buff->mtype=1;
    //strcpy(buff->mtext,"siema co tam\n");
    msgsnd(serverId,buff,sizeof(struct msgbuff),0);
    msgrcv(queueID,buff,sizeof(struct msgbuff),-1,0);
    //printf("siema");
    fflush(stdout);
    if(strcmp(buff->mtext,"full server")==0){
        printf("too many clients");
        exit(0);
    }
}

void list(int serverId){
    time_t time1;
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    time(&time1);
    buff->timeinfo= localtime(&time1);
    buff->clientKey=key;
    buff->mtype=2;
    msgsnd(serverId,buff,sizeof(struct msgbuff),0);
    msgrcv(queueID,buff,sizeof(struct msgbuff),2,0);
    printf("%s\n",buff->mtext);
    fflush(stdout);
}

void all(int serverId,char *message){
    time_t time1;
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    time(&time1);
    buff->timeinfo= localtime(&time1);
    buff->clientKey=key;
    buff->mtype=3;
    strcpy(buff->mtext,message);
    msgsnd(serverId,buff,sizeof(struct msgbuff),0);
}

void one(int serverId,int recipientId,char * message){
    time_t time1;
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    time(&time1);
    buff->timeinfo= localtime(&time1);
    buff->clientKey=key;
    buff->mtype=4;
    strcpy(buff->mtext,message);
    buff->auxiliaryKey=recipientId;
    msgsnd(serverId,buff,sizeof(struct msgbuff),0);
}

void stop(int serverId){
    time_t time1;
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    time(&time1);
    buff->timeinfo= localtime(&time1);
    buff->clientKey=key;
    buff->mtype=5;
    msgsnd(serverId,buff,sizeof(struct msgbuff),0);
    msgctl(key, IPC_RMID, NULL);
    exit(0);
}

void handler(int signum){
    stop(ftok(HOME,1));
}

void messageFromServer(){
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));

    while(msgrcv(queueID,buff,sizeof(struct msgbuff),-10,IPC_NOWAIT)>=0){
        //printf("wchodze\n");
        fflush(stdout);
        if(buff->mtype==5){
            stop(msgget(ftok(HOME,0),0));
        }else{
            //printf("siemasssdasdasdas\n");
            fflush(stdout);
//            printf("sent at %02d:%02d:%02d from: %d text:\n%s\n",
//                   (buff->timeinfo)->tm_hour,
//                   (buff->timeinfo)->tm_min,
//                   (buff->timeinfo)->tm_sec,
//                   buff->clientKey,
//                   buff->mtext);
            printf("%s %d",buff->mtext,buff->clientKey);
        }
    }
}

int main(){
    srand(time(NULL));
    key=ftok(HOME,rand()%1023+1);
    queueID=msgget(key,IPC_CREAT | 0666);
    int serverId=msgget(ftok(HOME,1),0);
    init(serverId);
    char command[1024];
    signal(SIGINT,handler);
    while(fgets(command,1024,stdin)){
        printf("%s",command);
        messageFromServer();
        char *iterator;
        iterator=strtok(command," ");
        if(strcmp(iterator,"LIST")==0){
            list(serverId);
        }else if(strcmp(iterator,"2ALL")==0){
            iterator=strtok(NULL," ");
            all(serverId,iterator);
        }else if(strcmp(iterator,"2ONE")==0){
            iterator=strtok(NULL," ");
            int recipientId=atoi(iterator);
            iterator=strtok(NULL," ");
            one(serverId,recipientId,iterator);
        }else if(strcmp(iterator,"STOP")==0){
            stop(serverId);
        }
    }
    return 0;
}