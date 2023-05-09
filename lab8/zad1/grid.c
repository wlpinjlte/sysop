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
pthread_t *t;
typedef struct {
    int i;
    int j;
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
    int i=arg->i;
    int j=arg->j;
    char *tmp;
    while(1){
        arg->dst[i*grid_width+j]= is_alive(i,j,arg->src);
        pause();
        tmp=arg->src;
        arg->src=arg->dst;
        arg->dst=tmp;
    }
}

void create_threads(char *src, char *dst){
    signal(SIGUSR1, handler);
    t=malloc(sizeof(pthread_t)*grid_width*grid_height);
    for(int i=0;i<grid_height;i++){
        for(int j=0;j<grid_width;j++){
            Arguments *arg=malloc(sizeof(Arguments));
            arg->i=i;
            arg->j=j;
            arg->src=src;
            arg->dst=dst;
            pthread_create(&t[i*grid_width+j],NULL,thread_function,(void*)arg);
        }
    }
}

void update_grid_new(char *src,char *dst){
    for(int i=0;i<grid_width*grid_height;i++){
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