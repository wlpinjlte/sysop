#include"semaphore_helper.h"


int create_semaphore(char *file,int i){
    int semid=semget(ftok(getenv("HOME"),file[0]),1,0666|IPC_CREAT);
    if(semctl(semid, 0, SETVAL, i) == -1) {
        perror("Creating a semaphore failed on semctl");
        return -1;
    }
    return semid;
}

int open_semaphore(char *file){
    return semget(ftok(getenv("HOME"),file[0]),1,0);
}

void unlink_semaphore(char *file){
    int semid= open_semaphore(file);
    semctl(semid,0,IPC_RMID);
}

void add_to_semaphore(int semaphore){
    struct sembuf oper={0,1,0};
    semop(semaphore,&oper,1);
}

void subtract_from_semaphore(int semaphore){
    struct sembuf oper={0,-1,0};
    semop(semaphore,&oper,1);
}