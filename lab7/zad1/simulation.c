#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <string.h>

#include "queue_manager.h"
#include "share_memory.h"
#include "semaphore_helper.h"
#define HOME getenv("HOME")
#define CHAIRS 10
#define BARBERS 5
#define CLIENTS 20
int semaphore_chairs;
int semaphore_queue;
int semaphore_barbers;
int buffer;

void create_semaphore_helper(){
    semaphore_chairs= create_semaphore("3",CHAIRS);
    semaphore_queue= create_semaphore("2",0);
    semaphore_barbers= create_semaphore("1",0);
    buffer= create_semaphore("4",1);
}
void unlink_semaphore_helper(){
    unlink_semaphore("1");
    unlink_semaphore("2");
    unlink_semaphore("3");
    unlink_semaphore("4");
}
int main(){
    char *memory= add_memory(HOME,1024);
    memory[0]='\0';
    unlink_semaphore_helper();
    create_semaphore_helper();
    for(int i=0;i<BARBERS;i++){
        if(fork()==0){
            execl("barber","main",NULL);
        }
    }
    printf("Barbers added\n");
    fflush(stdout);
    for(int i=0;i<CLIENTS;i++){
        if(fork()==0){
            execl("client","main",NULL);
        }
    }
    printf("clients added\n");
    fflush(stdout);
    while(wait(NULL)>0);
    delete_memory(HOME);
    printf("Simulation end!\n");
    fflush(stdout);
    return 0;
}