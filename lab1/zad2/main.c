#include <stdlib.h>
#include "library.c"
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/times.h>
#include <dlfcn.h>
#include <ctype.h>
#include <time.h>
int getInt(char *c){
    int i=0;
    while(c[i]<48||c[i]>57){
        i+=1;
    }
    int size=i;
    while(c[i]>47&&c[i]<58){
        i+=1;
    }
    int temp=size;
    size=i-size;
    i=temp;
    char *l=malloc(sizeof(char)*(size+1));
    int lcounter=0;
    while(c[i]>47&&c[i]<58){
        l[lcounter]=c[i];
        i+=1;
        lcounter+=1;
    }
    l[lcounter]='\0';
    int toReturn=atoi(l);

    return toReturn;
}
char* getFileName(char*c,int a){
    int i=a;
    int size=0;
    while(c[i]!=32){
        size+=1;
        i+=1;
    }
    i=a;
    char *fileName=calloc(size,sizeof(char));
    int fileCounter=0;
    while(c[i]!=32){
        fileName[fileCounter]=c[i];
        i+=1;
        fileCounter+=1;
    }
    fileName[fileCounter]='\0';
    return fileName;
}

int main(){
    struct Array array;
//    array=create(20);
    char *command=malloc(200*sizeof(char));
    char *result=malloc(sizeof(char)*(100));

    for(;;) {
        char *command=malloc(200*sizeof(char));
        char *result=malloc(sizeof(char)*(100));
        fgets(command, 200, stdin);
        int size = strlen(command);
        command[size] = '\0';
        int i=0;
        int j=0;
        printf("%s", command);
        for(i=0;i<size;i++){
            if(command[i]!=32){
                result[j]=command[i];
                j+=1;
            }else{
                result=realloc(result,j*sizeof(char));
//                result[j]='\0';
                printf("rezultat: %s\n",result);
                if(strcmp(result,"init")==0){
//                    printf("%d", getInt(command));
                    array=create(getInt(command));
                }else if(strcmp(result,"count")==0){
                    char *getfile=getFileName(command,j+1);
//                    printf("%s", getfile);
                    getfile[strlen(getfile)-1]='\0';
                    wcFunction(array,getfile);
                }else if(strcmp(result,"show")==0){
//                    printf("%d\n",getInt(command));
                    printf("%s",findByIndex(array, getInt(command)));
                }else if(strcmp(result,"delete")==0){
                    freeBlock(array, getInt(command));
                }else if(strcmp(result,"destroy")==0){
                    freeArray(array);
                }else{
                    break;
                }
            }
        }
        free(result);
        free(command);
//        printf("%s", findByIndex(array,0));
    }
    return 0;
}

