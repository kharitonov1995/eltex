#include "../include/manager.h"

void initCurses() {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
}

void initWindow(int nRows, int nCols, int startX, int startY, 
					char *title, panel *p) {
	
	p->box = newwin(nRows, nCols, startY, startX);
	box(p->box, 0, 0);
	p->window = derwin(p->box, nRows - 10, nCols - 10, startY + 1, startX + 1);
	keypad(p->window, TRUE);
	
	if (title != NULL) {
		/*TODO*/
	}
}

void initMenu(panel *p, List *head) {
	int sizeL = sizeList(head), i;
	char *tempPath = malloc(sizeof(char) * MAX_PATH);
	List *list;
	
	p->items = (ITEM **) calloc(sizeL + 1, sizeof(ITEM *));
	for(i = 0, list = head; list != NULL; i++, list = list->next) {
		sprintf(tempPath, "%s%c%s", p->path, '/',  list->data);
		if (isDirectory(tempPath)) {
			p->items[i] = new_item(list->data, _DIR);
		} else {
			p->items[i] = new_item(list->data, _FILE);
		}
		memset(tempPath, '\0', strlen(tempPath));
	}
	
	p->menu = new_menu(p->items);
	set_menu_win(p->menu, p->box);
	set_menu_sub(p->menu, p->window);
	post_menu(p->menu);
}

void delMenu(panel *p, List *head) {
	int i;
	List *list;
	unpost_menu(p->menu);
	free_menu(p->menu);
	for(i = 0, list = head; list != NULL; i++, list = list->next) {
		free_item(p->items[i]);
	}
	
}

List *getFilesCurDir(panel *p) {
	List *head = NULL, *list = NULL, *temp = NULL;
	
	if (getcwd(p->path, sizeof(p->path)) != NULL)
		/*wprintw(p->box, p->path);*/
		
	p->dir = opendir(p->path);
	if (p->dir != NULL) {
		while ((p->ent = readdir(p->dir)) != NULL) {
			if (strcmp(p->ent->d_name, ".") == 0) continue;
			if (list == NULL) {
				list = initList(p->ent->d_name);
				head = list;
			} else
				list = addElem(p->ent->d_name, list);
			if (strcmp(p->ent->d_name, "..") == 0) temp = list;
		}
		closedir(p->dir);
	} else {
		return NULL;
	}
	
	head = swap(head, head, temp);
	
	return head;
}

/*открытие только текстовых файлов*/
void execFile(char *path, char *fileName) {
	pid_t pid;
	int status;
	const char *prog = "/bin/nano";
	char *fullPath = malloc(sizeof(char) * MAX_PATH);
	static char *argc[] = {"nano", "", NULL};
	
	sprintf(fullPath, "%s%c%s", path, '/', fileName);
	argc[1] = fullPath;
	
	pid = fork();
	if (pid == 0) {
		execv(prog, argc);
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
