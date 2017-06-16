#include "../include/manager.h"

void initBox(panel *p) {
	p->box = newwin(NLINES + 10, NCOLS + 10, x, y);
	box(p->box, '|', '-');
	wrefresh(p->box);
}

void initWindow(panel *p) {
	p->window = derwin(p->box, NLINES + 7, NCOLS + 7, x + 2, y + 1);
	wrefresh(p->window);
}

List *getFilesCurDir(panel *p) {
	List *head = NULL, *list = NULL;
	
	if (getcwd(p->path, sizeof(p->path)) != NULL)
		wprintw(p->window, p->path);
		
	p->dir = opendir(p->path);
	if (p->dir != NULL) {
		while ((p->ent = readdir(p->dir)) != NULL) {
			if (list == NULL) {
				list = initList(p->ent->d_name);
				head = list;
			} else
				list = addElem(p->ent->d_name, list);
		}
		closedir(p->dir);
	} else {
		perror("");
		return NULL;
	}
	
	wrefresh(p->window);
	return head;
}
