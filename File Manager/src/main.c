#include "../lib/manager.c"
#include "../lib/list.c"

void sigWinch(int signo) {
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int main() {
	panel panels[2];
	List *head = NULL;
	int ch;
	char *itemName;
	
	initscr();
	signal(SIGWINCH, sigWinch);
	cbreak();
	noecho();
	curs_set(0);
	initBox(&panels[0]);
	initWindow(&panels[0]);
	keypad(panels[0].window, TRUE);
	
	head = getFilesCurDir(&panels[0]);
	initMenu(&panels[0], head);
	wrefresh(panels[0].box);
	
	while((ch = wgetch(panels[0].window)) != KEY_F(1)) {
		switch(ch) {
			case KEY_DOWN:
				menu_driver(panels[0].menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(panels[0].menu, REQ_UP_ITEM);
				break;
			case 10:
				/*mvwprintw(panels[0].box, y, x, "Item selected is : %s", 
						item_name(current_item(panels[0].menu)));
					*/
				clrtoeol();
				itemName = (char*) item_name(current_item(panels[0].menu));
				if (strcmp(item_description(current_item(panels[0].menu)), _DIR) == 0) {
					changeDirectory((char*) &panels[0].path, itemName);
					
					delMenu(&panels[0], head);
					head = getFilesCurDir(&panels[0]);
					initMenu(&panels[0], head);
				} else if (strcmp(item_description(current_item(panels[0].menu)), _FILE) == 0) {
					execFile((char*) &panels[0].path, itemName);
				}
				break;
		}
		wrefresh(panels[0].window);
	}
	
	delMenu(&panels[0], head);
	clearList(head);
	endwin();
	
	return 0;
}
