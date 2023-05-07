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
#define QUEUE_SIZE 10
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
    srand(time(NULL));
    char *memory= add_memory(HOME,BUFF_SIZE);
    if(strlen(memory)>=QUEUE_SIZE){
        printf("queue full\n");
        exit(0);
    }
    open_semaphore_helper();
    subtract_from_semaphore(semaphore_queue);
    subtract_from_semaphore(buffer);
    char haircut=rand()%128;
    printf("client:%d haircut number %d\n",getpid(),haircut);
    fflush(stdout);
    queue_push(memory,haircut);
    add_to_semaphore(buffer);
    add_to_semaphore(semaphore_barbers);
    subtract_from_semaphore(semaphore_chairs);
    printf("client:%d done\n",getpid());
    fflush(stdout);
    return 1;
}