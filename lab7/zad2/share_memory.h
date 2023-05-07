#ifndef SYSOP_SHARE_MEMORY_H
#define SYSOP_SHARE_MEMORY_H
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char *add_memory(char * file,int size);
int remove_memory(char *memory);
int delete_memory(char *file);

#endif //SYSOP_SHARE_MEMORY_H
