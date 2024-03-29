#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>

#include "queue_manager.h"
#include "share_memory.h"
#include "semaphore_helper.h"
#define HOME getenv("HOME")
#define BUFF_SIZE 1024
int semaphore_chairs;
int semaphore_queue;
int semaphore_barbers;
int buffer;

void open_semaphore_helper(){
    semaphore_barbers= open_semaphore("1");
    semaphore_queue= open_semaphore("2");
    semaphore_chairs= open_semaphore("3");
    buffer= open_semaphore("4");
}

int main(){
    srand(getpid());
    char *memory;
    memory=add_memory(HOME,BUFF_SIZE);
    printf("client:%d start\n",getpid());
    fflush(stdout);

    open_semaphore_helper();
//    printf("size:%d\n",semctl(semaphore_queue,0,GETVAL,0));
    if(semctl(semaphore_queue,0,GETVAL,0)==0){
        printf("client:%d queue full\n",getpid());
        exit(0);
    }
    subtract_from_semaphore(semaphore_queue);
    subtract_from_semaphore(buffer);
    char haircut=rand()%10+1;
    printf("client:%d haircut number %d\n",getpid(),haircut);
    fflush(stdout);
    queue_push(memory,haircut);
    add_to_semaphore(buffer);
    subtract_from_semaphore(semaphore_chairs);
    add_to_semaphore(semaphore_barbers);
    printf("client:%d done\n",getpid());
    fflush(stdout);
    remove_memory(memory);
    return 0;
}