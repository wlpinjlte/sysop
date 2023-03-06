#include<stdio.h>
#include <stdlib.h>
#include"library.c"
#include<string.h>
int main(){
    char *elo=malloc(10*sizeof(char));
    struct Array array=create(10);
    printf("\n");
    wcFunction(array,"main.c");
    printf("\n");
    printf("%s",findByIndex(array,0));
    for(;;) {
        fgets(elo, 10, stdin);
        int size = strlen(elo);
        elo[size] = '\n';
        elo[size + 1] = '\0';
        printf("%s", elo);
    }
    return 0;
}

