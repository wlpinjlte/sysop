#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#define BUFF_SIZE 1024
#define PATH "/tmp/integral_tmp"

double f(double x){
    return 4/(x*x+1);
}

int main(int argc,char **argv){
    if(argc!=4){
        printf("invalid number of arguments");
        return 1;
    }
    char buff[BUFF_SIZE];
    double dx=strtod(argv[1],NULL);
    int n=atoi(argv[2]);
    int i=atoi(argv[3]);
    double length_of_react=1.0/n;
    long steps=length_of_react/dx;
    double result=0;
    double j=length_of_react*i;
    for(long g=0;g<steps-1;g++){
        result+=dx*f(j);
        j+=dx;
    }
    result+=f(j)*(length_of_react*(i+1)-j);
    snprintf(buff,BUFF_SIZE,"%lf ",result);
    int file=open(PATH,O_WRONLY);
    write(file,buff,strlen(buff));
    close(file);
    return 0;
}