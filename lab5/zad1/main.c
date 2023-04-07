#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define BUFFSIZE 1024

int main(int argc,char **argv){
    if(argc==2){
        FILE* mail;
        if(strcmp(argv[1],"nadawca")==0){
            mail=popen("mail | tail -n +2 | sort -k3d","w");
        }else if(strcmp(argv[1],"data")==0){
            mail=popen("mail | tail -n +2","w");
        }else{
            printf("invalid argument");
            return 1;
        }
        fwrite("exit",sizeof(char),5,mail);
        pclose(mail);
    }else if(argc==4){
        char command[1024];
        snprintf(command,1024,"mail -s \"%s\" %s",argv[2],argv[1]);
//        printf("%s\n",command);
        FILE* mail=popen(command,"w");
        fwrite(argv[3],sizeof(char),strlen(argv[3]),mail);
        pclose(mail);
    }else{
        printf("invalid number of arguments\n");
        return 1;
    }
    return 0;
}