#include "share_memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
int get_memory(char * file,int size){
    int memoryid=shm_open(file,O_CREAT|O_RDWR,0666);
    if(memoryid==-1){
        return -1;
    }
    if(ftruncate(memoryid,size)==-1){
        printf("ftruncate error!\n");
    }
    return memoryid;
}

int remove_memory(char *memory){
    if(shmdt(memory)==-1){
        return 0;
    }
    return 1;
}

int delete_memory(char *memory){
    if(shm_unlink(memory) ==-1){
        return 0;
    }
    return 1;
}

char *add_memory(char *file,int size){
    int memory_id= get_memory(file,size);
    if(memory_id==-1){
        printf("get memory error!\n");
    }
    char *memory;
    memory=(char*) mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, memory_id, 0);
    return memory;
}