#include <dirent.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

#include <curses.h>
#include <termios.h>

typedef struct _editor {
	WINDOW *box;
	WINDOW *editWindow;
	WINDOW *label;
} Editor;

void initCurses();
void initWindow(Editor*, int, int, int, int);

