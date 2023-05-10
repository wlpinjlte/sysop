#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc,char **argv)
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;
    int n=atoi(argv[1]);
    printf("%d\n",n);
	init_grid(foreground);
    create_threads(foreground,background,n);
	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);

		// Step simulation
		update_grid_new(foreground, background);
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
