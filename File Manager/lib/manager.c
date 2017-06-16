#include "../include/manager.h"

void initBox(panel *p) {
	p->box = newwin(NCOLS + 10, NLINES + 20, x, y);
	box(p->box, 0, 0);
	mvprintw(NLINES + 9, x + 1, "F1 to exit");
	wrefresh(p->box);
}

void initWindow(panel *p) {
	p->window = derwin(p->box, NCOLS + 5, NLINES + 15, x + 2, y + 1);
	box(p->window, 0, 0);
}

List *getFilesCurDir(panel *p) {
	List *head = NULL, *list = NULL;
	
	if (getcwd(p->path, sizeof(p->path)) != NULL)
		//wprintw(p->box, p->path);
		
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
		return NULL;
	}
	
	return head;
}
