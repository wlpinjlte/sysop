#ifndef ZAD1_LIBRARY_H
#define ZAD1_LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
struct Array{
    int *size_of_array;
    int *curr_index;
    char **blocksArray;
};


struct Array create(int size);
void wcFunction(struct Array array,char *name_of_file);
char *findByIndex(struct Array array,int index);
void freeBlock(struct Array array,int index);
void freeArray();


#endif //ZAD1_LIBRARY_H
