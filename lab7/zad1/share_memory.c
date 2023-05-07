#include "share_memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
char *add_memory(char * file,int size){
    int memory_id=shmget(ftok(file,0),size,0666|IPC_CREAT);
    char *memory;
    memory=shmat(memory_id,NULL,0);
    return memory;
}

int remove_memory(char *memory){
    if(shmdt(memory)==-1){
        return 0;
    }
    return 1;
}

int delete_memory(char *memory){
    int memory_id=shmget(ftok(memory,0),0,0666|IPC_CREAT);
    if(shmctl(memory_id,IPC_RMID,NULL)==-1){
        return 0;
    }
    return 1;
}