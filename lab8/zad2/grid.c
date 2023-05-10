#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include<stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
const int grid_width = 30;
const int grid_height = 30;
int number_of_threads;
pthread_t *t;
typedef struct {
    int start;
    int end;
    char *src;
    char *dst;
}Arguments;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void handler(int signum){}

void *thread_function(void *_args){
    Arguments *arg=(Arguments*)_args;
    int start=arg->start;
    int end=arg->end;
    char *tmp;
    while(1){
        for(int i=start;i<end;i++){
            arg->dst[i]=is_alive(i/grid_width,i%grid_width,arg->src);
        }
        pause();
        tmp=arg->src;
        arg->src=arg->dst;
        arg->dst=tmp;
    }
}

void create_threads(char *src, char *dst,int n){
    number_of_threads=n;
    signal(SIGUSR1, handler);
    t=malloc(sizeof(pthread_t)*n);
    int step;
    if(grid_width*grid_height%n==0){
        step=grid_width*grid_height/n;
    }else{
        step=grid_width*grid_height/n+1;
    }
    int start=0;
    int end=step;
    for(int i=0;i<n-1;i++){
        Arguments *arg=malloc(sizeof(Arguments));
        arg->start=start;
        arg->end=end;
        arg->src=src;
        arg->dst=dst;
        pthread_create(&t[i],NULL,thread_function,(void*)arg);
        start=end;
        end=start+step;
    }
    Arguments *arg=malloc(sizeof(Arguments));
    arg->start=start;
    arg->end=grid_width*grid_height;
    arg->src=src;
    arg->dst=dst;
    pthread_create(&t[n-1],NULL,thread_function,(void*)arg);
}

void update_grid_new(char *src,char *dst){
    for(int i=0;i<number_of_threads;i++){
        pthread_kill(t[i],SIGUSR1);
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}