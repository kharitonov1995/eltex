#include <dirent.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>

#include <curses.h>
#include <menu.h>
#include <termios.h>

#include "../include/list.h"

const int NCOLS = 10, NLINES = 10, x = 1, y = 1;

typedef struct st {
	char path[64];
	WINDOW *window;
	WINDOW *box;
	DIR *dir;
	struct dirent *ent;
} panel;

void initBox(panel*);
void initWindow(panel*);
List *getFilesCurDir(panel*);
