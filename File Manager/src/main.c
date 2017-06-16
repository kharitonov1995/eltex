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
	int sizeL = 0;
	
	initscr();
	signal(SIGWINCH, sigWinch);
	cbreak();
	noecho();
	refresh();
	
	initBox(&panels[0]);
	initWindow(&panels[0]);
	
	head = getFilesCurDir(&panels[0]);
	sizeL = sizeList(head);
	list = head;
	items = (ITEM **) calloc(sizeL, sizeof(ITEM *));
	
	for(int i = 0; i < sizeL; i++) {
		items[i] = new_item(list->data, list->data);
		list = list->next;
	}
	
	menu = new_menu((ITEM **)items);
	set_menu_win(menu, panels[0].box);
    set_menu_sub(menu, panels[0].window);

	getch();
	endwin();
	
	
	return 0;
}
