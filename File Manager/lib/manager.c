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

int getFilesCurDir(panel *p) {
	if (getcwd(p->path, sizeof(p->path)) != NULL)
		wprintw(p->window, p->path); 
		
	p->dir = opendir(p->path);
	if (p->dir != NULL) {
		while ((p->ent = readdir(p->dir)) != NULL)
			wprintw(p->window, p->ent->d_name);
		closedir(p->dir);
	} else {
		perror("");
		return EXIT_FAILURE;
	}
	
	wrefresh(p->window);
	return 0;
}
