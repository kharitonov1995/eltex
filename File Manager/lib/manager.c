#include "../include/manager.h"

void initBox(panel *p) {
	p->box = newwin(NLINES + 10, NCOLS+ 20, x, y);
	box(p->box, 0, 0);
	mvprintw(NLINES + 9, x + 1, "F1 to exit");
}

void initWindow(panel *p) {
	p->window = derwin(p->box, NLINES + 5, NCOLS + 15, x + 2, y + 1);
}

void initMenu(panel *p, List *head) {
	int sizeL = sizeList(head), i;
	char tempPath[255];
	List *list;
	
	printf("\n");
	
	p->items = (ITEM **) calloc(sizeL + 1, sizeof(ITEM *));
	for(i = 0, list = head; list != NULL; i++, list = list->next) {
		sprintf(tempPath, "%s%c%s", p->path, '/',  list->data);
		if (isDirectory(tempPath)) {
			p->items[i] = new_item(list->data, _DIR);
		} else {
			p->items[i] = new_item(list->data, _FILE);
		}
		memset(tempPath, '\0', sizeof(tempPath));
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
	for(i = 0, list = head; list != NULL; i++, list = list->next)
		free_item(p->items[i]);
}

List *getFilesCurDir(panel *p) {
	List *head = NULL, *list = NULL, *temp = NULL;
	
	if (getcwd(p->path, sizeof(p->path)) != NULL)
		//wprintw(p->box, p->path);
		
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

void changeDirectory(char *curPath, char *distDirectory) {
	sprintf(curPath, "%s%c%s", curPath, '/', distDirectory);
	chdir(curPath);
	getcwd(curPath, MAX_PATH);
}

int isDirectory(char *path) {
	struct stat statBuf;
	
	if (stat(path, &statBuf) != 0)
		return -1;
		
	return S_ISDIR(statBuf.st_mode);
}
