#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

int main(int argc,char **argv){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return 1;
    }
    DIR *  directory=opendir(cwd);
    if(directory==NULL){
        printf("directory don't open");
        return 1;
    }
    struct dirent* single_object;
    struct stat stats;
    long long full_size=0;
    while(single_object=readdir(directory)){
        stat(single_object->d_name,&stats);
        if (S_ISDIR(stats.st_mode))
            continue;
        printf("%s %ld\n",single_object->d_name,stats.st_size);
        full_size+=stats.st_size;
    }
    printf("full size:%lld\n",full_size);
    return 0;
}