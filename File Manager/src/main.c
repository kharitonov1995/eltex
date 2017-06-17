#include "../lib/manager.c"
#include "../lib/list.c"

void sigWinch(int signo) {
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int main() {
	panel panels[2];
	ITEM **items;
	MENU *menu;
	List *head = NULL, *list = NULL;
	int sizeL = 0, i;
	int ch;
	
	initscr();
	signal(SIGWINCH, sigWinch);
	cbreak();
	noecho();
	curs_set(0);
	initBox(&panels[0]);
	initWindow(&panels[0]);
	keypad(panels[0].window, TRUE);
	
	head = getFilesCurDir(&panels[0]);
	sizeL = sizeList(head);
	printf("\n");
	items = (ITEM **) calloc(sizeL + 1, sizeof(ITEM *));
	for(i = 0, list = head; list != NULL; i++, list = list->next) {
		items[i] = new_item(list->data, "file");
	}
	
	menu = new_menu(items);
	set_menu_win(menu, panels[0].box);
    set_menu_sub(menu, panels[0].window);
	post_menu(menu);
	wrefresh(panels[0].box);
	
	while((ch = wgetch(panels[0].window)) != KEY_F(1)) {
		switch(ch) {
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
			case 10:
				clrtoeol();
				mvwprintw(panels[0].box, y, x, "Item selected is : %s", 
						item_name(current_item(menu)));
				pos_menu_cursor(menu);
				wrefresh(panels[0].box);
				break;
		}
		wrefresh(panels[0].window);
	}
	
	unpost_menu(menu);
	free_menu(menu);
	for(i = 0, list = head; list != NULL; i++, list = list->next)
		free_item(items[i]);
	clearList(head);
	endwin();
	
	return 0;
}
