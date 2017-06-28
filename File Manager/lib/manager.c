#include <dirent.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>

#include "../include/manager.h"

void initCurses() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
}

void initPanel(panel *p, int startY, int startX) {
	int my, mx;
	getmaxyx(stdscr, my, mx);
	p->windowMenu = newwin(my - 5, mx / 2 - 1, startY, startX);
	box(p->windowMenu, 0, 0);
	p->startX = startX;
	p->startY = startY;
	p->selectItem = 0;
	getFilesDir(p);
	keypad(p->windowMenu, TRUE);
}

void initPanels(panel *p, int startY, int startX, int countPanels) {
	int i, mx = 0;
	
	for (i = 0; i < countPanels; i++) {
		if (i == 0) {
			initPanel(&p[i], startY, startX);
		} else if (i > 0) {					
			mx = getmaxx(p[i - 1].windowMenu);
			initPanel(&p[i], startY, mx + 1);
		}
		
		drawMenuPanel(
				&p[i], 
				startY + 1, 
				startX + 1, 
				p[i].selectItem, 
				p[i].items);
	}
}

void drawMenuPanel(panel *p, int startY, int startX, int selectItem, char **items) {
	int i, line, maxLines = 0;
	char *tempPath, *tempString;
	
	tempPath = malloc(sizeof(char) * MAX_PATH);
	tempString = malloc(sizeof(char) * 32);
	
	maxLines = getmaxy(p->windowMenu);
	
	if (maxLines > p->countItems) {
		p->countShowItems = p->countItems;
	} else {
		p->countShowItems = maxLines - 3;
	}
	
	for (i = 0, line = startY; i < p->countShowItems; i++, line++) {
		
		sprintf(tempPath, "%s%c%s", p->path, '/', items[i]);
				
		if (i == selectItem) 
			wattron(p->windowMenu, A_STANDOUT);
			
		if (isDirectory(tempPath)) {
			wattron(p->windowMenu, A_BOLD);
			sprintf(tempString, "%c%s", '/', items[i]);
			printToWindow(p->windowMenu, tempString, line, startX, COLOR_WHITE);
			wattroff(p->windowMenu, A_BOLD);
			memset(tempString, '\0', strlen(tempString));
		} else {
			if (!isExecFile(tempPath))
				printToWindow(p->windowMenu, items[i], line, startX, COLOR_BLUE);
			else
				printToWindow(p->windowMenu, items[i], line, startX, COLOR_GREEN);
		}
	
		wattroff(p->windowMenu, A_STANDOUT);
		memset(tempPath, '\0', strlen(tempPath));
	}
	wrefresh(p->windowMenu);
	free(tempPath);
	free(tempString);
}

void destructPanel(panel *p) {
	int i;
	
	delwin(p->windowMenu);
	for (i = 0; i < p->countItems; i++)
		free(p->items[i]);
	free(p->items);
}

void printToWindow(WINDOW *win, char *text, int startY, int startX, int color) {
	if(win == NULL)
		win = stdscr;
	
	wattron(win, COLOR_PAIR(color));
	mvwprintw(win, startY, startX, "%s", text);
	wattroff(win, COLOR_PAIR(color));
	wrefresh(win);
}

int getCountFilesDir(char *path) {
	int count = 0;
	DIR *dir;
	struct dirent *ent = NULL;
	
	if (getcwd(path, MAX_PATH) == NULL) {
		perror("Path is null: ");
		exit(1);
	}
	
	dir = opendir(path);
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, ".") == 0) 
				continue; 
			count++;
		}
		closedir(dir);
	} else {
		perror("Can not open directory: ");
		endwin();
		exit(2);
	}
	
	return count;
}

void getFilesDir(panel *p) {
	int indx = 0, i = 0;
	struct dirent **ent = NULL;
	
	if (getcwd(p->path, MAX_PATH) == NULL) {
		perror("Path is null: ");
		exit(1);
	}
	
	p->countItems = scandir(p->path, &ent, NULL, alphasort);
	
	if (p->countItems < 0) {
		perror("Fails scan directory: ");
		exit(2);
	}
	
	p->items = calloc(p->countItems - 1, sizeof(char*));
	for (i = 0; i < p->countItems - 1; i++)
		p->items[i] = calloc(32, sizeof(char));
	i = 0;
	while (indx < p->countItems) {
		if (strcmp(ent[indx]->d_name, ".") != 0) {
			sprintf(p->items[i], "%s", ent[indx]->d_name);
			i++;
		}
		indx++;
	}
	p->countItems--;
}

/*открытие только текстовых файлов*/
void execFile(char *path, char *fileName, int _isExecFile) {
	pid_t pid;
	int status;
	char *prog;
	char *fullPath;
	char *argv1, *argv2;
	
	fullPath = malloc(sizeof(char) * MAX_PATH);
	sprintf(fullPath, "%s%c%s", path, '/', fileName);
	if (!_isExecFile) {
		prog = "/home/artem/Документы/eltex/Text Editor/editor";
		argv1 = "editor";
		argv2 = fileName;
	} else {
		prog = fullPath;
		argv1 = fileName;
		argv2 = "";
	}
	
	pid = fork();
	if (pid == 0) {
		if (execl(prog, argv1, argv2, NULL) < 0) {
			perror("exec");
			exit(EXIT_FAILURE);
		}
	}
	wait(&status);
	free(fullPath);
}

void changeDirectory(char *curPath, char *distDirectory) {
	sprintf(curPath, "%s%c%s", curPath, '/', distDirectory);
	if (chdir(curPath) != 0)
		exit(EXIT_FAILURE);
	if (getcwd(curPath, MAX_PATH) == NULL) 
		exit(EXIT_FAILURE);
}

int isDirectory(char *path) {
	struct stat statBuf;
	
	if (stat(path, &statBuf) != 0)
		return -1;	
	return S_ISDIR(statBuf.st_mode);
}

int isExecFile(char *path) {
	struct stat statBuf;
	
	if (stat(path, &statBuf) != 0)
		return -1;
	return (S_IXUSR & statBuf.st_mode) > 0 ? 1 : 0;
}
