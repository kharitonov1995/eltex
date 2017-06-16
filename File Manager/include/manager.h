#include <curses.h>
#include <dirent.h> 
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

const int NCOLS = 10, NLINES = 10, x = 1, y = 1;

typedef struct st {
	char *nameDir;
	DIR *dir;
	WINDOW *window;
	WINDOW *box;
} panel;

void initBox(panel*);
void initWindow(panel*);
void getFilesCurDir(panel*);
