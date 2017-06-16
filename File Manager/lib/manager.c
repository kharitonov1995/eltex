#include "../include/manager.h"

void initBox(panel *p) {
	p->box = newwin(NLINES, NCOLS, x, y);
	box(p->box, '|', '-');
	wrefresh(p->box);
	printw("Press any key to continue...");
}

void initWindow(panel *p) {
	p->window = derwin(p->box, NLINES - 2, NCOLS - 2, x + 1, y + 1);
	wprintw(p->window, "HELLO");
	wrefresh(p->window);
}
