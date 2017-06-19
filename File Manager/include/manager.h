#include <dirent.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <curses.h>
#include <menu.h>
#include <termios.h>

#include "../include/list.h"

const int MAX_PATH = 255;
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
void initWindow(int, int, int, int, char*, panel*);
void initMenu(panel*, List*);
void delMenu(panel*, List*);
void changeDirectory(char*, char*);
void execFile(char*, char*);
List *getFilesCurDir(panel*);
int isDirectory(char *);
