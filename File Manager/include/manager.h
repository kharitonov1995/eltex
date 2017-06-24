#include <curses.h>

#include "../include/list.h"

const int MAX_PATH = 128, LENGTH_NAME = 16;
const char _DIR[] = "dir", _FILE[] = "file";

typedef struct st {
	char path[128];
	WINDOW *windowMenu;
	int startX;
	int startY;
	int countItems;
	int selectItem;
	char **items;
} panel;

void initCurses();
void initPanel(panel*, int, int);
void initMenuPanel(panel*);
void drawMenuPanel(panel*, int, int, int, char**);
void printToWindow(WINDOW*, char*, int, int, int);
void delMenuPanel(panel*);
void destructPanel(panel*);
void execFile(char*, char*);
int getCountFilesDir(char*);
void getFilesDir(panel*);
void changeDirectory(char*, char*);
int isDirectory(char *);
