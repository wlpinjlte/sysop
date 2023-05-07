#ifndef SYSOP_SEMAPHORE_HELPER_H
#define SYSOP_SEMAPHORE_HELPER_H

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#define HOME getenv("HOME")
int create_semaphore(char *file,int i);
int open_semaphore(char *file);
void unlink_semaphore(char *file);
void add_to_semaphore(int semaphore);
void subtract_from_semaphore(int semaphore);
#endif //SYSOP_SEMAPHORE_HELPER_H