#include <stdio.h>
#include <string.h>

#include "queue_manager.h"

char queue_pop(char *queue){
    if(queue_empty(queue)==1){
        printf("ERROR:empty queue");
        return '\0';
    }
    char to_return=queue[0];
    memcpy(queue,queue+1,strlen(queue)+1);
    return to_return;
}

void queue_push(char *queue,char c){
    if(queue_full(queue)==1){
        printf("ERROR: full queue");
    }else{
        queue[strlen(queue)-1]=c;
        queue[strlen(queue)]='\0';
    }
}

int queue_full(char *queue){
    if(strlen(queue)==BUFSIZ){
        return 1;
    }
    return 0;
}

int queue_empty(char *queue){
    if(strlen(queue)==0){
        return 1;
    }
    return 0;
}