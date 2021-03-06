#define _GNU_SOURCE

#include "../include/manager.h"

const int MAX_PATH = 128; /**< It is maximum length of absolute path. */
const int LENGTH_NAME = 255; /**< It is length name of elements in menu. */
const int SIZE_COPY_BLOCK = 90;/**< It is size of showing blocks of copied data. */

void initCurses() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	
	start_color();
	init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(8, COLOR_BLACK, COLOR_WHITE);
}

WINDOW *initWindowInfo() {
	WINDOW *win = NULL;
	WINDOW *winF1 = NULL, *winTab = NULL, *winCopy = NULL;
	int mx, cols = 10, startX = 1;
	
	mx = getmaxx(stdscr);
	win = newwin(1, mx - 2, 0, 1);
	
	winF1 = derwin(win, 1, cols, 0, startX);
	wprintw(winF1, "F1 - Exit");
	wvline(winF1, '|', 1);
	
	startX += cols;
	cols += cols;
	
	winTab = derwin(win, 1, cols, 0, startX);
	wprintw(winTab, "Tab - Switch panels");
	wvline(winTab, '|', 1);
	
	startX += cols;
	
	winCopy = derwin(win, 1, cols, 0, startX);
	wprintw(winCopy, "F5 - Copy");
	wvline(winCopy, '|', 1);
	
	wbkgd(win, COLOR_PAIR(8));
	wrefresh(win);
	
	return win;
}

void initPanel(panel *p, int startY, int startX) {
	int my, mx;
	getmaxyx(stdscr, my, mx);
	p->windowMenu = newwin(my - 5, mx / 2 - 1, startY, startX);
	box(p->windowMenu, 0, 0);
	p->beginPos = 0;
	p->startX = startX;
	p->startY = startY;
	p->selectItem = 0;
	getFilesDir(p);
	p->endPos = getMaxShowLines(p);
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
				startY, 
				startX + 1, 
				p[i].selectItem, 
				p[i].items);
	}
}

void drawMenuPanel(panel *p, int startY, int startX, int selectItem, char **items) {
	int i, line;
	char *tempPath, *tempString;
	
	tempPath = malloc(sizeof(char) * MAX_PATH);
	tempString = malloc(sizeof(char) * LENGTH_NAME);
	
	for (i = p->beginPos, line = startY; i < p->endPos; i++, line++) {
		
		sprintf(tempPath, "%s%c%s", p->path, '/', items[i]);
						
		if (i == selectItem) 
			wattron(p->windowMenu, A_STANDOUT);
			
		if (isDirectory(tempPath)) {
			sprintf(tempString, "%c%s", '/', items[i]);
			printToWindow(p->windowMenu, tempString, line, startX, COLOR_BLACK, A_BOLD);
		} else if (isExecFile(tempPath)) {
			sprintf(tempString, "%s%s", "*", items[i]);
			printToWindow(p->windowMenu, tempString, line, startX, COLOR_GREEN, A_BOLD);
		} else {
			printToWindow(p->windowMenu, items[i], line, startX, COLOR_BLUE, 0);
		}
		memset(tempString, '\0', strlen(tempString));
		wattroff(p->windowMenu, A_STANDOUT);
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

int getMaxShowLines(panel *p) {
	int maxLines;
	maxLines = getmaxy(p->windowMenu);
	if ((maxLines - 1) > p->countItems) {
		return p->countItems;
	} else {
		return maxLines - 2;
	}
}

void printToWindow(WINDOW *win, char *text, int startY, int startX, int color, int attr) {
	if(win == NULL)
		win = stdscr;
	
	if (attr != 0)
		wattron(win, attr);
		
	wattron(win, COLOR_PAIR(color));
	mvwprintw(win, startY, startX, "%s", text);
	wattroff(win, COLOR_PAIR(color));
	wattroff(win, attr);
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
		p->items[i] = calloc(LENGTH_NAME, sizeof(char));
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

char *trimSpaces(char *str) {
	char *end;
	
	while(isspace(*str))
		str++;

	if(*str == 0)
		return str;

	end = str + strlen(str) - 1;

	while(end > str && isspace(*end))
		end--;
		
	*(end + 1) = '\0';
	
	return str;
}

void *threadCopy(void *arg) {
	struct argsThread *args = (struct argsThread*) arg;
	char *buf;
	FILE *fileWrite;
	const int sizeBuf = 1024;
	
	buf = malloc(sizeBuf * sizeof(char));
	fileWrite = fopen(args->targetPath, "w+");
	if (fileWrite == NULL) {
		perror("Can not open file to write");
		pthread_exit(0);
	}
	
	while (fread(buf, sizeof(char), sizeBuf, args->file) > 0) {
		fwrite(buf, sizeof(char), sizeBuf, fileWrite);
		memset(buf, 0, sizeBuf);
	}
	
	fclose(fileWrite);
	fclose(args->file);
	free(buf);
	pthread_exit(0);
}

void *threadDraw(void *arg) {
	struct argsThread *args = (struct argsThread*) arg;
	struct stat statBuf;
	WINDOW *copy = NULL;
	int fileSize = 0, procent = 0, col = 1;
	int i, j, mx = 0;
	
	mx = getmaxx(args->win);
	copy = derwin(args->win, 2, mx - 4 , 6, 2);
	werase(copy);
	stat(args->sourcePath, &statBuf);
	fileSize = statBuf.st_size;
	procent =  fileSize / SIZE_COPY_BLOCK;
	curs_set(0);
	
	for (i = 0; i < fileSize; i += procent, col++) {
		for (j = 0; j < procent; j++) { }
		mvwprintw(copy, 0, col, "=");
		wrefresh(copy);
	}
	
	wprintw(copy, "Success");
	curs_set(1);
	delwin(copy);
	pthread_exit(0);
}

void copyForm(panel *p, char *fileCopy) {
	pthread_t _threadCopy;
	pthread_t _threadDraw;
	FORM *form;
	FIELD *field[3];
	WINDOW *copyWin;
	int mx, my, ch;
	struct argsThread *arg = NULL;
	char *tempPath;
	
	arg = malloc(sizeof(struct argsThread));
	arg->targetPath = calloc(MAX_PATH, sizeof(char));
	arg->sourcePath = calloc(MAX_PATH, sizeof(char));
	
	sprintf(arg->sourcePath, "%s%c%s", p->path, '/', fileCopy);
	
	getmaxyx(p->windowMenu, my, mx);
	copyWin = derwin(p->windowMenu, 10, mx - 2, my / 2 - 2, 1);
	werase(copyWin);
	
	getmaxyx(copyWin, my, mx);
	field[0] = new_field(1, mx / 2, 0, 1, 0, 0);
	field[1] = new_field(1, mx - 5, 2, 0, 0, MAX_PATH);
	field[2] = NULL;
	
	field_opts_off(field[0], O_ACTIVE);
	set_field_buffer(field[0], 0, "The path to copy:"); 
	
	field_opts_off(field[1], O_STATIC | O_BLANK);
	set_field_back(field[1], A_UNDERLINE);
	set_field_buffer(field[1], 0, arg->sourcePath);
	set_max_field(field[1], MAX_PATH);
	
	form = new_form(field);
	set_form_win(form, copyWin);
	set_form_sub(form, derwin(copyWin, my - 4, mx - 4, 2, 2));
	post_form(form);
	
	mvwhline(copyWin, 0, 0, '-', mx);
	mvwhline(copyWin, my - 1, 0, '-', mx);
	
	form_driver(form, REQ_END_FIELD);
	wrefresh(copyWin);
	
	keypad(copyWin, TRUE);
    curs_set(1); 
	while(ch != 27) {
		ch = wgetch(copyWin);
		switch(ch) {
			case KEY_LEFT:
				form_driver(form, REQ_PREV_CHAR);
				break;
			case KEY_RIGHT:
				form_driver(form, REQ_NEXT_CHAR);
				break;
			case KEY_BACKSPACE:
				form_driver(form, REQ_DEL_PREV);
				break;
			case KEY_DC:
				form_driver(form, REQ_DEL_CHAR);
				break;
			case KEY_ENTER:
			case 10:
				if (form_driver(form, REQ_VALIDATION) == E_OK) {
					tempPath = trimSpaces(field_buffer(field[1], 0));
					if (strcmp(tempPath, arg->sourcePath) == 0) {
						sprintf(
								arg->targetPath,
								"%s%ccopy_%s", 
								p->path, 
								'/',
								fileCopy);
					} else  {
						sprintf(arg->targetPath, "%s", tempPath);
					}
				} else {
					break;
				}
				arg->win = copyWin;
				arg->file = fopen(arg->sourcePath, "r");
				if (arg->file != NULL) {
					pthread_create(&_threadCopy, NULL, threadCopy, arg);
					pthread_create(&_threadDraw, NULL, threadDraw, arg);
					
					pthread_join(_threadCopy, NULL);
					pthread_join(_threadDraw, NULL);
				} else {
					perror("Connot open file for copy");
					exit(EXIT_FAILURE);
				}
				break;
			default: 
				form_driver(form, ch);
				break;
		}
		wrefresh(copyWin);
	}
    curs_set(0);
    
	free(arg->targetPath);
	free(arg->sourcePath);
	free(arg);
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
