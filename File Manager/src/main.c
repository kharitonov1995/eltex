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
	getch();
	endwin();
	
	return 0;
}


	/*DIR *dir;
	struct dirent *ent;
	char currentDir[128];
	
	if (getcwd(currentDir, sizeof(currentDir)) != NULL)
		printf("%s\n", currentDir); 
	
	if ((dir = opendir (currentDir)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			printf ("%s\n", ent->d_name);
		}
		closedir (dir);
		} else {
			perror ("");
		return EXIT_FAILURE;
		}*/
