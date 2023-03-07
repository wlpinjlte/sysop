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

double timeDifference(clock_t t1, clock_t t2){
    return ((double)(t2 - t1) / sysconf(_SC_CLK_TCK));
}

void writeResult(clock_t start, clock_t end,struct tms t_start,struct tms t_end){
    printf("\tREAL_TIME: %fl\n", timeDifference(start,end));
    printf("\tUSER_TIME: %fl\n", timeDifference(t_start.tms_utime, t_end.tms_utime));
    printf("\tSYSTEM_TIME: %fl\n", timeDifference(t_start.tms_stime, t_end.tms_stime));
}

int main(){
    struct Array array;
//    array=create(20);
//    char *command=calloc(200,sizeof(char));
//    char *result=calloc(200,sizeof(char));
    clock_t timeStart,timeEnd;
    static struct tms tmsStart,tmsEnd;
    int flag=1;
    for(;;) {
        printf(">>");
        timeStart=0;
        timeEnd=0;
        timeStart=times(&tmsStart);
        char *command=malloc(200*sizeof(char));
        char *result=malloc(sizeof(char)*(200));
        fgets(command, 200, stdin);
        int size = strlen(command);
        command[size] = '\0';
        int i=0;
        int j=0;
//        printf("%s", command);
        for(i=0;i<size;i++){
            if(command[i]!=32){
                result[j]=command[i];
                j+=1;
            }else{
                result=realloc(result,j*sizeof(char));
//                result[j]='\0';
//                printf("rezultat: %s\n",result);
                if(strcmp(result,"init")==0){
//                    printf("%d", getInt(command));
                    array=create(getInt(command));
                    break;
                }else if(strcmp(result,"count")==0){
                    char *getfile=getFileName(command,j+1);
//                    printf("%s", getfile);
                    getfile[strlen(getfile)-1]='\0';
                    wcFunction(array,getfile);
                    break;
                }else if(strcmp(result,"show")==0){
//                    printf("%d\n",getInt(command));
                    printf("%s",findByIndex(array, getInt(command)));
                    break;
                }else if(strcmp(result,"delete")==0) {
                    freeBlock(array, getInt(command));
                    break;
                }else if(strcmp(result,"destroy")){
                    freeArray(array);
                }else if(strcmp(result,"exit")){
                    flag=0;
                }
                else{
                    printf("Unknown command");
                    break;
                }
            }
//            if(i==size-1){
//                printf("siema");
//            }
        }
        free(result);
        free(command);
        timeEnd=times(&tmsEnd);
        writeResult(timeStart, timeEnd,
                    tmsStart, tmsEnd);

        if(flag==0){
            break;
        }
//        printf("%s", findByIndex(array,0));
    }
    return 0;
}

