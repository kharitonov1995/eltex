#include "../lib/manager.c"

void sigWinch(int signo) {
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int main() {
	panel panels[2];
	
	initscr();
	signal(SIGWINCH, sigWinch);
	cbreak();
	noecho();
	refresh();
	
	initBox(&panels[0]);
	initWindow(&panels[0]);
	getFilesCurDir(&panels[0]);
	
	getch();
	endwin();
	
	return 0;
}
