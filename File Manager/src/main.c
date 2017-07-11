#include "../include/manager.h"

/*F1 to exit*/
int main() {
	const int startX = 1, startY = 1, COUNT_PANELS = 2;
	panel *panels = NULL;
	WINDOW *windowInfo = NULL;
	int ch = 0, currentPanel = 0, i = 0;
	int selectItem = 0, _isExecFile = 0, page = 0;
	
	initCurses();
	refresh();
	windowInfo = initWindowInfo();
	panels = malloc (sizeof(panel) * COUNT_PANELS);
	initPanels(panels, startY, startX, COUNT_PANELS);
	
	while(1) {
		ch = wgetch(panels[currentPanel].windowMenu);
		if (ch == KEY_F(1)) break;
		switch(ch) {
			case KEY_DOWN:
				if (panels[currentPanel].selectItem < panels[currentPanel].endPos - 1) 
					panels[currentPanel].selectItem++;
			break;
			case KEY_UP:
				if (panels[currentPanel].selectItem > panels[currentPanel].beginPos) 
					panels[currentPanel].selectItem--;
			break;
			case KEY_NPAGE:
			case KEY_PPAGE:
				page = getMaxShowLines(&panels[currentPanel]);
				
				if (ch == KEY_NPAGE) {
					if (panels[currentPanel].endPos == panels[currentPanel].countItems)
						break;
					
					panels[currentPanel].beginPos = panels[currentPanel].endPos;
					
					if (panels[currentPanel].countItems < (panels[currentPanel].beginPos + page)) 
						panels[currentPanel].endPos = panels[currentPanel].countItems;
					else 
						panels[currentPanel].endPos += page;
				} else {
					if (panels[currentPanel].beginPos == 0) 
						break;
					
					panels[currentPanel].endPos = panels[currentPanel].beginPos;	
					
					if ((panels[currentPanel].beginPos - page) < 0) 
						panels[currentPanel].beginPos = 0;
					else 
						panels[currentPanel].beginPos -= page;
				}
				werase(panels[currentPanel].windowMenu);
				box(panels[currentPanel].windowMenu, 0, 0);
				panels[currentPanel].selectItem = panels[currentPanel].beginPos;
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
				selectItem = panels[currentPanel].selectItem;
				if (!isDirectory(panels[currentPanel].items[selectItem])) {	
					copyForm(
							&panels[currentPanel],
							panels[currentPanel].items[selectItem]);
				}
				destructPanel(&panels[currentPanel]);
				initPanel(
						&panels[currentPanel],
						panels[currentPanel].startY,
						panels[currentPanel].startX);
			break;
		}
		
		drawMenuPanel(
				&panels[currentPanel], 
				startY, 
				startX + 1, 
				panels[currentPanel].selectItem, 
				panels[currentPanel].items);
	}
	
	for (i = 0; i < COUNT_PANELS; i++)
		destructPanel(&panels[i]);
	free(panels);
	
	delwin(windowInfo);
	endwin();
	curs_set(1);
	return 0;
}
