#include"semaphore_helper.h"
#include <stdlib.h>
int create_semaphore(char *file,int i){
    key_t key=ftok(getenv("HOME"),file[0]);
    if(key==-1){
        printf("ftok probelm! error\n");
        return -1;
    }
    int semid=semget(key,1,0666|IPC_CREAT);
    if(semid==-1){
        printf("semid not created\n");
        return -1;
    }
    if(semctl(semid, 0, SETVAL, i) == -1) {
        printf("Creating a semaphore failed on semctl");
        return -1;
    }
    return semid;
}

int open_semaphore(char *file){
    key_t key=ftok(getenv("HOME"),file[0]);
    if(key==-1){
        printf("ftok error!\n");
        return -1;
    }
    int semid=semget(key,1,0);
    return semid;
}

void unlink_semaphore(char *file){
    int semid= open_semaphore(file);
    if(semctl(semid,0,IPC_RMID) == -1) {
        printf("semctl unlink problem\n");
    }
}

void add_to_semaphore(int semaphore){
    struct sembuf oper={0,1,0};
    semop(semaphore,&oper,1);
}

void subtract_from_semaphore(int semaphore){
    struct sembuf oper={0,-1,0};
    semop(semaphore,&oper,1);
}