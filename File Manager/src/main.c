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
	int ch, currentPanel = 0, i, selectItem = 0, _isExecFile = 0;
	
	initCurses();
	refresh();
	panels = malloc (sizeof(panel) * COUNT_PANELS);
	initPanels(panels, startY, startX, COUNT_PANELS);
	while(1) {
		ch = wgetch(panels[currentPanel].windowMenu);
		if (ch == KEY_F(1)) break;
		switch(ch) {
			case KEY_DOWN:
				if (panels[currentPanel].selectItem < panels[currentPanel].countShowItems - 1) 
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
					
					destructPanel(&panels[currentPanel]);
					
					initPanel(
							&panels[currentPanel],
							panels[currentPanel].startY,
							panels[currentPanel].startX);
				} else {
					endwin();
					_isExecFile = isExecFile(
								panels[currentPanel].items[selectItem]);
					execFile(
							panels[currentPanel].path,
							panels[currentPanel].items[selectItem],
							_isExecFile);
					initCurses();
				}
			break;
			case 9:
				currentPanel = !currentPanel;
			break;
			case KEY_F(5):
				
			break;
		}
		
		drawMenuPanel(
				&panels[currentPanel], 
				startY + 1, 
				startX + 1, 
				panels[currentPanel].selectItem, 
				panels[currentPanel].items);
	}
	
	for (i = 0; i < COUNT_PANELS; i++)
		destructPanel(&panels[i]);
	free(panels);
	
	endwin();
	curs_set(1);
	return 0;
}
