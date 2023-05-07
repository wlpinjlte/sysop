#include"semaphore_helper.h"
#include <stdlib.h>
sem_t* create_semaphore(char *file,int i){
    sem_t *s = sem_open(file, O_CREAT | O_EXCL, 0666, i);
    if (s == SEM_FAILED) {
        printf("sem create problem\n");
        return NULL;
    }
    return s;
}

sem_t* open_semaphore(char *file){
    sem_t* s = sem_open(filename, 0);
    if (s == SEM_FAILED) {
        printf("sem open probelm\n");
        return NULL;
    }
    return s;
}

void unlink_semaphore(char *file){
    sem_unlink(file);
}

void add_to_semaphore(sem_t* semaphore){
    sem_wait(semaphore);
}

void subtract_from_semaphore(sem_t* semaphore){
    sem_post(semaphore);
}