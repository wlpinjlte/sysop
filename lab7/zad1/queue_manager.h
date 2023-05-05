#ifndef SYSOP_QUEUE_MANAGER_H
#define SYSOP_QUEUE_MANAGER_H

char queue_pop(char *queue);
void queue_push(char *queue,char c);
int queue_full(char *queue);
int queue_empty(char *queue);

#endif //SYSOP_QUEUE_MANAGER_H
