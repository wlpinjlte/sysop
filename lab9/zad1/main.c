#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#define NUMBER_OF_REINDEER 9
#define NUMBER_OF_ELVES 10
#define NUMBER_OF_WAITING_ELVES 3

int reindeer=0;
int elves=0;
long *waiting_elves_ids;
int counter=0;

pthread_mutex_t data_access_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waiting_reindeer=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waiting_elves=PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t santa_condition=PTHREAD_COND_INITIALIZER;
pthread_cond_t reindeer_condition=PTHREAD_COND_INITIALIZER;
pthread_cond_t elves_condition=PTHREAD_COND_INITIALIZER;

void *santa(void *arg){
    printf("Santa created\n");
    while(counter<3){
        printf("Santa go to sleep\n");
        pthread_mutex_lock(&data_access_mutex);
        while(reindeer<NUMBER_OF_REINDEER&&elves<NUMBER_OF_WAITING_ELVES){
            pthread_cond_wait(&santa_condition,&data_access_mutex);
        }
        if(reindeer==NUMBER_OF_REINDEER){
            printf("Santa wake up: delivering presents\n");
//            pthread_mutex_unlock(&data_access_mutex);
            sleep(2 + rand() % 3);
//            pthread_mutex_lock(&data_access_mutex);
            counter+=1;
            reindeer=0;
            pthread_mutex_lock(&waiting_reindeer);
            pthread_cond_broadcast(&reindeer_condition);
            pthread_mutex_unlock(&waiting_reindeer);
        }else if(elves==NUMBER_OF_WAITING_ELVES){
//            pthread_mutex_unlock(&data_access_mutex);
            sleep(1 + rand() % 2);
//            pthread_mutex_lock(&data_access_mutex);
            printf("Santa wake up: helping elves,%ld %ld %ld ID\n",waiting_elves_ids[0],waiting_elves_ids[1],waiting_elves_ids[2]);
            elves=0;
            pthread_mutex_lock(&waiting_elves);
            pthread_cond_broadcast(&elves_condition);
            pthread_mutex_unlock(&waiting_elves);
        }
        pthread_mutex_unlock(&data_access_mutex);
    }
    printf("santa closing\n");
    return NULL;
}

void *reindeer_thread_function(void *arg){
    printf("Reindeer number %ld created\n",pthread_self());
    while(1){
        printf("Reindeer number %ld on vaction\n",pthread_self());
        sleep(5+(rand()%6));
        pthread_mutex_lock(&data_access_mutex);
        reindeer+=1;
        if(reindeer==NUMBER_OF_REINDEER){
            printf("Reindeer number %ld wake up santa\n",pthread_self());
        }else{
            printf("Reindeer number %ld waiting for santa\n",pthread_self());
        }
        pthread_cond_broadcast(&santa_condition);
        pthread_mutex_unlock(&data_access_mutex);
        pthread_mutex_lock(&waiting_reindeer);
        pthread_cond_wait(&reindeer_condition,&waiting_reindeer);
        pthread_mutex_unlock(&waiting_reindeer);
    }
}

void *elf(void *arg){
    printf("Elf number %ld created\n",pthread_self());
    while(1){
        sleep(2 + (rand() % 4));
        pthread_mutex_lock(&data_access_mutex);
        if(elves<NUMBER_OF_WAITING_ELVES){
            waiting_elves_ids[elves]=pthread_self();
            elves+=1;
            if(elves==NUMBER_OF_WAITING_ELVES){
                printf("Elf number %ld wake up santa\n",pthread_self());
            }else{
                printf("Elf number %ld waiting for santa\n",pthread_self());
            }
            pthread_cond_broadcast(&santa_condition);
            pthread_mutex_unlock(&data_access_mutex);
            pthread_mutex_lock(&waiting_elves);
            pthread_cond_wait(&elves_condition,&waiting_elves);
            pthread_mutex_unlock(&waiting_elves);
        }else{
            printf("Elf number %ld handle thing himself\n",pthread_self());
            pthread_mutex_unlock(&data_access_mutex);
        }
    }
}

int main(){
    waiting_elves_ids=malloc(sizeof(int)*3);
    pthread_t *reindeers_threads=malloc(sizeof(pthread_t)*NUMBER_OF_REINDEER);
    pthread_t *elves_threads=malloc(sizeof(pthread_t)*NUMBER_OF_ELVES);
    pthread_t santa_thread;
    pthread_create(&santa_thread,NULL,santa,NULL);
    for(int i=0;i<NUMBER_OF_REINDEER;i++){
        pthread_create(&reindeers_threads[i],NULL,reindeer_thread_function,NULL);
    }
    for(int i=0;i<NUMBER_OF_ELVES;i++){
        pthread_create(&elves_threads[i],NULL,elf,NULL);
    }
    pthread_join(santa_thread,NULL);
    for(int i=0;i<NUMBER_OF_REINDEER;i++){
        pthread_cancel(reindeers_threads[i]);
    }
    for(int i=0;i<NUMBER_OF_ELVES;i++){
        pthread_cancel(elves_threads[i]);
    }
    return 0;
}