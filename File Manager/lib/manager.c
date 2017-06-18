#include "../include/manager.h"

void initBox(panel *p) {
	p->box = newwin(NLINES + 10, NCOLS+ 20, x, y);
	box(p->box, 0, 0);
	mvprintw(NLINES + 9, x + 1, "F1 to exit");
}

void initWindow(panel *p) {
	p->window = derwin(p->box, NLINES + 5, NCOLS + 15, x + 2, y + 1);
}

List *getFilesCurDir(panel *p) {
	List *head = NULL, *list = NULL;
	
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
		}
		closedir(p->dir);
	} else {
		return NULL;
	}
	
	return head;
}

int isDirectory(char *path) {
	struct stat statBuf;
	
	if (stat(path, &statBuf) != 0)
		return -1;
		
	return S_ISDIR(statBuf.st_mode);
}
