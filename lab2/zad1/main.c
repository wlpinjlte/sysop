#include<stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
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
//    printf("%d",argc);
    if(argc!=5){
        printf("zla ilosc argumentow");
        return 1;
    }
//    zad1a(argv);
    zad1b(argv);
    return 0;
}