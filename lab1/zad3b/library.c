#include "library.h"
#include <ctype.h>
#define SIZE 150
struct Array create(int size){
    struct Array array;
    array.blocksArray=(char**) calloc(size, sizeof(char*));
    array.curr_index=malloc(sizeof(int));
    array.size_of_array=malloc(sizeof(int));
    *array.curr_index=-1;
    *array.size_of_array=size;
    return array;
}

void wcFunction(struct Array array,char *name_of_file){
    char *command=calloc(SIZE,sizeof(char));
    char filePath[]="./tmp/temp.txt";
    *array.curr_index+=1;
//    printf("%d",*array.curr_index);
    int i=0;
    for(;i<SIZE;i++){
        command[i]=0;
    }
    strcpy(command,"wc ");
    strcat(command,name_of_file);
    strcat(command," > ");
    strcat(command,filePath);
//    printf("%s\n",command);
    system(command);

    FILE *file=fopen(filePath,"r");
    if(file==NULL){
        perror("Cannot open file");
        exit(1);
    }
    int size;
    fseek(file, 0, SEEK_END); // seek to end of file
    size = ftell(file); // get current file pointer
    fseek(file, 0, SEEK_SET); // seek back to beginning of file
    // proceed with allocating memory and reading the file
    array.blocksArray[*array.curr_index]=calloc((size+1),sizeof(char));
    fread(array.blocksArray[*array.curr_index],sizeof(char),size,file);
//    printf("\n");
//    printf("%s",array.blocksArray[*array.curr_index]);
}

char *findByIndex(struct Array array,int index){
    return array.blocksArray[index];
}

void freeBlock(struct Array array,int index){
//    free(array.blocksArray[index]);
    memmove(&array.blocksArray[index],&array.blocksArray[index+1],sizeof(char*)*(*array.size_of_array-index-1));
    *array.curr_index-=1;
}

void freeArray(struct Array array){
    free(array.blocksArray);
    free(array.size_of_array);
    free(array.curr_index);
}