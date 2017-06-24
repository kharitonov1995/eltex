#include "../lib/manager.c"
/*
void sigWinch(int signo) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}
*/

/*F1 to exit*/
int main() {
	const int startX = 1, startY = 1, COUNT_PANELS = 2;
	panel *panels;
	int ch, currentPanel = 0, mx = 0, i, selectItem = 0;
	
	initCurses();
	refresh();
	panels = malloc (sizeof(panel) * COUNT_PANELS);
	
	for (i = 0; i < COUNT_PANELS; i++) {
		if (i == 0) {
			initPanel(&panels[i], startY, startX);
		} else if (i > 0) {					
			mx = getmaxx(panels[i - 1].windowMenu);
			initPanel(&panels[i], startY, mx + 1);
		}
		
		drawMenuPanel(
				&panels[i], 
				startY + 1, 
				startX + 1, 
				panels[i].selectItem, 
				panels[i].items);
	}
	
	while(1) {
		ch = wgetch(panels[currentPanel].windowMenu);
		if (ch == KEY_F(1)) break;
		switch(ch) {
			case KEY_DOWN:
				if (panels[currentPanel].selectItem < panels[currentPanel].countItems - 1) 
					panels[currentPanel].selectItem++;
			break;
			case KEY_UP:
				if (panels[currentPanel].selectItem > 0) 
					panels[currentPanel].selectItem--;
			break;
			case KEY_NPAGE:
				
				break;
			case KEY_PPAGE:
				
				break;
			case 10:
				selectItem = panels[currentPanel].selectItem;
				if (isDirectory(panels[currentPanel].items[selectItem])) {
					changeDirectory(
							panels[currentPanel].path,
							panels[currentPanel].items[selectItem]);
							
					delwin(panels[currentPanel].windowMenu);
					
					initPanel(
							&panels[currentPanel],
							panels[currentPanel].startY,
							panels[currentPanel].startX);
				}
			break;
			case 9:
				currentPanel = !currentPanel;
			break;
		}
		
		drawMenuPanel(
				&panels[currentPanel], 
				startY + 1, 
				startX + 1, 
				panels[currentPanel].selectItem, 
				panels[currentPanel].items);
	}
	/*
	for (i = 0; i < COUNT_PANELS; i++)
		destructPanel(&panels[i]);
	clearList(head);*/
	endwin();
	/*char **items, tempPath[128];
	int count = 0;
	printf("===%s\n", tempPath);
	count = getCountFilesDir(tempPath);
	items = getFilesDir(tempPath, count);
	
	for (i = 0; i < count; i++) {
		printf("%s\n", items[i]);
	}*/
	
	return 0;
}
