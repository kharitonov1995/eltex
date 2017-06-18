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

const int NCOLS = 10, NLINES = 10, x = 1, y = 1, MAX_PATH = 255;
const char _DIR[] = "dir", _FILE[] = "file";

typedef struct st {
	char path[255];
	WINDOW *window;
	WINDOW *box;
	ITEM **items;
	MENU *menu;
	DIR *dir;
	struct dirent *ent;
} panel;

void initBox(panel*);
void initWindow(panel*);
void initMenu(panel*, List*);
void delMenu(panel*, List*);
void changeDirectory(char*, char*);
List *getFilesCurDir(panel*);
int isDirectory(char *);
