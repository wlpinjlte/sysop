#include <stdlib.h>
#include "library.c"
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/times.h>
#include <dlfcn.h>
#include <ctype.h>
#include <time.h>
int main(){
    char *command=malloc(10*sizeof(char));
    struct Array array=create(10);
    printf("\n");
    wcFunction(array,"main.c");
    printf("\n");
    printf("%s",findByIndex(array,0));
    for(;;) {
        fgets(command, 100, stdin);
        int size = strlen(command);
        command[size] = '\n';
        command[size + 1] = '\0';
        printf("%s", command);
    }
    return 0;
}

