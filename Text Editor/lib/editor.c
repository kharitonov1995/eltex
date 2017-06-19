#include "../include/editor.h"

void initCurses() {
	setlocale(LC_ALL, "rus");
	initscr();
	cbreak();
	noecho();
}

void initWindow(Editor *e, int nRows, int nCols, int startY, int startX) {
	e->box = newwin(nRows + 3, nCols + 10, startY, startX);
	e->editWindow = derwin(e->box, nRows, nCols, startY + 1, startX + 1);
	
	box(e->box, 0, 0);
	wrefresh(e->box);
	keypad(e->editWindow, TRUE);
}
