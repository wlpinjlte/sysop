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
    char *command=malloc(20*sizeof(char));
    struct Array array=create(10);
    printf("\n");
    wcFunction(array,"main.c");
    wcFunction(array,"main");
    wcFunction(array,"library.c");
    printf("\n");
//    printf("%s",findByIndex(array,*array.curr_index));
    printf("%s", findByIndex(array,0));
    printf("%s",findByIndex(array,1));
    printf("%s", findByIndex(array,2));
    freeBlock(array,1);
    printf("%s", findByIndex(array,1));
    printf("%s",findByIndex(array,0));
    printf("%s",findByIndex(array,2));
    wcFunction(array,"library.c");
    printf("%s", findByIndex(array,2));
    wcFunction(array,"main");
    printf("%s", findByIndex(array,3));
    freeBlock(array,2);
    wcFunction(array,"library.o");
    printf("%s", findByIndex(array,3));
    for(;;) {
        fgets(command, 20, stdin);
        int size = strlen(command);
        command[size] = '\n';
        command[size] = '\0';
        char *result=malloc(sizeof(char)*size);
        int i=0;
        int j=0;
        for(i=0;i<size;i++){
            if(command[i]!=32){
                result[j]=command[i];
                j+=1
            }else{
                result[j]='\0';
                if(strcmp(result,"init")){

                }else if(strcmp(result,"count")){

                }else if(strcmp(result,"show")){

                }else if(strcmp(result,"delete")){

                }else if(strcmp(result,"destroy")){

                }
            }

        }
        printf("%s", command);
    }
    return 0;
}

