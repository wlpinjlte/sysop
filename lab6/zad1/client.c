#include "header.h"

key_t key;

int queueID;

void init(int serverId){
    time_t time1;
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    time(&time1);
    buff->timeinfo=*localtime(&time1);
    printf("%d\n",key);
    buff->clientKey=key;
    buff->mtype=1;
    msgsnd(serverId,buff,sizeof(struct msgbuff),0);
    msgrcv(queueID,buff,sizeof(struct msgbuff),1,0);
    if(strcmp(buff->mtext,"full server")==0){
        printf("too many clients\n");
        exit(0);
    }
    printf("id:%d\n",buff->auxiliaryId);
}

void list(int serverId){
    time_t time1;
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    time(&time1);
    buff->timeinfo= *localtime(&time1);
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
    buff->timeinfo= *localtime(&time1);
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
    buff->timeinfo= *localtime(&time1);
    buff->clientKey=key;
    buff->mtype=4;
    strcpy(buff->mtext,message);
    buff->auxiliaryId=recipientId;
    msgsnd(serverId,buff,sizeof(struct msgbuff),0);
}

void stop(int serverId){
    time_t time1;
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));
    time(&time1);
    buff->timeinfo= *localtime(&time1);
    buff->clientKey=key;
    buff->mtype=5;
    msgsnd(serverId,buff,sizeof(struct msgbuff),0);
    msgctl(key, IPC_RMID, NULL);
    exit(0);
}

void handler(int signum){
    stop(msgget(ftok(HOME,1),0));
}

void messageFromServer(){
    struct msgbuff *buff;
    buff=malloc(sizeof(struct msgbuff));

    while(msgrcv(queueID,buff,sizeof(struct msgbuff),-10,IPC_NOWAIT)>=0){
        if(buff->mtype==5){
            printf("STOP\n");
            fflush(stdout);
            msgsnd(msgget(ftok(HOME,1),0),buff,sizeof (struct msgbuff),0);
            stop(msgget(ftok(HOME,1),0));
        }else{
            printf("sent at %02d:%02d:%02d from: %d text:\n%s\n",
                   (buff->timeinfo).tm_hour,
                   (buff->timeinfo).tm_min,
                   (buff->timeinfo).tm_sec,
                   buff->auxiliaryId,
                   buff->mtext);
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
    while(1){
        messageFromServer();
        printf(">>");
        fgets(command,1024,stdin);
        if(strlen(command)>1){
            command[strlen(command)-1]=' ';
            command[strlen(command)]='\0';
        }
        printf("%s\n",command);
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