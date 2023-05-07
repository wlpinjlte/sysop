#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>

#include "queue_manager.h"
#include "share_memory.h"
#include "semaphore_helper.h"
#define QUEUE_SIZE 10
#define BUFF_SIZE 1024
#define HOME getenv("HOME")
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
    char *memory= add_memory(HOME,BUFF_SIZE);
    open_semaphore_helper();
    printf("Barber:%d\n",getpid());
    fflush(stdout);
    while(1){
        subtract_from_semaphore(semaphore_barbers);
        subtract_from_semaphore(buffer);
        char haircut= queue_pop(memory);
        add_to_semaphore(buffer);

        printf("barber:%d haircut numer:%d in progress\n",getpid(),haircut);
        fflush(stdout);
        sleep(haircut);
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
    fflush(stdout);
    remove_memory(memory);
    return 0;
}