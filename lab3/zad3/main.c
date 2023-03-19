#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
char exe_path[PATH_MAX];
int main(int argc,char ** argv){
    if(argc!=3){
        printf("zla liczba argumentow");
        return 1;
    }
    char *directory_path;
    directory_path=calloc(strlen(argv[1]),sizeof(char));
    strcpy(directory_path,argv[1]);
    DIR *directory=opendir(directory_path);

    struct dirent* single_object;
    struct stat stats;

    char *destination_path;
    realpath(argv[0],exe_path);
    while((single_object=readdir(directory))){
        if (strcmp(single_object->d_name, ".") == 0) continue;
        if (strcmp(single_object->d_name, "..") == 0) continue;

        destination_path=calloc(PATH_MAX,sizeof(char));
        strcpy(destination_path,directory_path);
        strcat(destination_path,"/");
        strcat(destination_path,single_object->d_name);

        stat(destination_path,&stats);
//        printf("%d\n",S_ISDIR(stats.st_mode));
//        printf("%s %s\n",destination_path,single_object->d_name);

        if (S_ISDIR(stats.st_mode)){
            if(fork()==0) {
                execl(exe_path,"main",destination_path,argv[2],NULL);
            }
        }else{
            FILE *file=fopen(destination_path,"r");
            char *read_buffer=calloc(strlen(argv[2]),sizeof(char));
            fread(read_buffer,sizeof(char),strlen(argv[2]),file);
            if(strcmp(read_buffer,argv[2])==0){
                printf("%s %d\n",destination_path,(int)getpid());
                fflush(NULL);
            }
            fclose(file);
        }
    }
    while (wait(NULL) > 0);
    return 0;
}