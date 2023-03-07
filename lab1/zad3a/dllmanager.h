#ifndef SYSOP_DLLMANAGER_H
#define SYSOP_DLLMANAGER_H

#ifdef USE_DLL
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#define WCLIB_SYMBOL_TABLE_SIZE 7

void load_dll(const char *path){
    void *handle = dlopen(filepath, RTLD_LAZY);

    if(handle==NULL){
        return;
    }

    *(void **) (&create) = dlsym(handle,"create");
    *(void **) (&wcFunction) = dlsym(handle,"wcFunction");
    *(void **) (&findByIndex) = dlsym(handle,"findByIndex");
    *(void **) (&freeBlock) = dlsym(handle,"freeBlock");
    *(void **) (&freeArray) = dlsym(handle,"freeArray");
}

#else
void load_dll(const char * file){}
#endif
#endif//SYSOP_DLLMANAGER_H
