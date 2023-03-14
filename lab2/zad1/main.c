#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/times.h>
double timeDifference(clock_t t1, clock_t t2){
    return ((double)(t2 - t1) / sysconf(_SC_CLK_TCK));
}

void writeResult(clock_t start, clock_t end,struct tms t_start,struct tms t_end){
    printf("\tREAL_TIME: %fl\n", timeDifference(start,end));
    printf("\tUSER_TIME: %fl\n", timeDifference(t_start.tms_utime, t_end.tms_utime));
    printf("\tSYSTEM_TIME: %fl\n", timeDifference(t_start.tms_stime, t_end.tms_stime));
}

void zad1a(char **argv){
    int we,wy;
    char c;
    we=open(argv[3],O_RDONLY);
    wy=open(argv[4],O_WRONLY|O_CREAT);
    while(read(we,&c,1)==1){
        if(c==*argv[1]){
            strncpy(&c,argv[2],1);
//            c=*argv[2];
        }
        write(wy,&c,1);
//        printf("%c",c);
    }
    close(we);
    close(wy);
}

void zad1b(char **argv){
    char c;
    FILE *we=fopen(argv[3],"r");
    FILE *wy=fopen(argv[4],"w");
    while(fread(&c,sizeof(char),1,we)==sizeof(char)){
        if(c==*argv[1]){
            strncpy(&c,argv[2],1);
        }
        fwrite(&c,sizeof(char),1,wy);
    }
    fclose(we);
    fclose(wy);
}

int main(int argc,char **argv){
    clock_t timeStart,timeEnd;
    static struct tms tmsStart,tmsEnd;
    if(argc!=5){
        printf("zla ilosc argumentow\n");
        return 1;
    }
    timeStart=0;
    timeEnd=0;
    timeStart=times(&tmsStart);
#ifndef SYS
    zad1a(argv);
//    printf("std");
#else
    zad1b(argv);
//    printf("sys");
#endif
    timeEnd=times(&tmsEnd);
    writeResult(timeStart, timeEnd,
                tmsStart, tmsEnd);
    return 0;
}