#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#define BLOCK_SIZE 1024
void reverse(char* buffer,int size_of_buffer){
    int i=0;
    char tmp;
    for(;i<size_of_buffer/2;i++){
        tmp=buffer[i];
        buffer[i]=buffer[size_of_buffer-1-i];
        buffer[size_of_buffer-1-i]=tmp;
    }
}
int main(int argc,char ** argv){
    if(argc!=3){
        printf("wrong number of arguments");
        return 1;
    }
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
    return 0;
}