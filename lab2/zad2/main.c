#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 1
#endif
void reverse(char* buffer,int size_of_buffer){
    int i=0;
    char tmp;
    for(;i<size_of_buffer/2;i++){
        tmp=buffer[i];
        buffer[i]=buffer[size_of_buffer-1-i];
        buffer[size_of_buffer-1-i]=tmp;
    }
}

double timeDifference(clock_t t1, clock_t t2){
    return ((double)(t2 - t1) / sysconf(_SC_CLK_TCK));
}

void writeResult(clock_t start, clock_t end,struct tms t_start,struct tms t_end){
    printf("\tREAL_TIME: %fl\n", timeDifference(start,end));
    printf("\tUSER_TIME: %fl\n", timeDifference(t_start.tms_utime, t_end.tms_utime));
    printf("\tSYSTEM_TIME: %fl\n", timeDifference(t_start.tms_stime, t_end.tms_stime));
}

int main(int argc,char ** argv){
    if(argc!=3){
        printf("wrong number of arguments");
        return 1;
    }

    clock_t timeStart,timeEnd;
    static struct tms tmsStart,tmsEnd;
    timeStart=0;
    timeEnd=0;
    timeStart=times(&tmsStart);
    FILE *read_file=fopen(argv[1],"r");
    FILE *write_file=fopen(argv[2],"w");
    int size_of_file=0;
    fseek(read_file, 0, SEEK_END);
    size_of_file = ftell(read_file);
    fseek(read_file, 0, SEEK_SET);
    char *buffer=calloc(size_of_file,sizeof(char));
    int index=0;
    while(!feof(read_file)){
        index+=fread(&buffer[index],sizeof(char),BLOCK_SIZE,read_file);
//        printf("%s\n",buffer);
//        printf("%d\n",index);
//        printf("\n");
    }
    reverse(buffer,strlen(buffer));
    fwrite(buffer,sizeof(char),size_of_file,write_file);
    timeEnd=times(&tmsEnd);
    writeResult(timeStart, timeEnd,
                tmsStart, tmsEnd);
    return 0;
}