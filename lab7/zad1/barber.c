#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>

#include "queue_manager.h"
#include "share_memory.h"
#include "semaphore_helper.h"
int semaphore_chairs;
int semaphore_queue;
int semaphore_barbers;
int buffer;
void open_semaphore(){
    semaphore_barbers= open_semaphore("1");
    semaphore_queue= open_semaphore("2");
    semaphore_chairs= open_semaphore("3");
    buffer= open_semaphore("4");
}
int main(){
    char *memory= add_memory(HOME,BUFF_SIZE);
    if(strlen(queue)>=QUEUE_SIZE){
        printf("queue full\n");
        exit(0);
    }
    open_semaphore();
    printf("Barber:%d\n",getpid());
    fflush(stdout);
    while(1){
        subtract_from_semaphore(semaphore_barbers);
        subtract_from_semaphore(buffer);
        char haircut= queue_pop(memory);
        add_to_semaphore(buffer);

        printf("barber:%d haircut numer:%d in progress\n",getpid(),haircut);
        fflush(stdout);
        sleep(10);
        printf("barber:%d haircut numer:%d done\n",getpid(),haircut);
        fflush(stdout);

        add_to_semaphore(semaphore_chairs);
        add_to_semaphore(semaphore_barbers);
        if(queue_empty(memory)){
            sleep(10);
            if(queue_empty(memory)){
                break;
            }
        }
    }
    printf("closing barber %d\n",getpid());
}