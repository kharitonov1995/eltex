#include <dirent.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include <curses.h>
#include <menu.h>
#include <termios.h>

#include "../include/list.h"

const int NCOLS = 10, NLINES = 10, x = 1, y = 1;
const char _DIR[] = "dir", _FILE[] = "file";

typedef struct st {
	char path[255];
	WINDOW *window;
	WINDOW *box;
	DIR *dir;
	struct dirent *ent;
} panel;

void initBox(panel*);
void initWindow(panel*);
List *getFilesCurDir(panel*);
int isDirectory(char *);
