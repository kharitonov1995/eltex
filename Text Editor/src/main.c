#include "../include/editor.h"

int main(int argc, char **argv) {
	const int x = 1, y = 1, NROWS = 20, NCOLS = 90, BUF_LENGTH = 255;
	int ch, curX = 0, curY = 0, i, line = 0;
	char *fileName = argv[1];
	char *buf = malloc(sizeof(char) * BUF_LENGTH);
	FILE *file;
	Editor editor;
	
	if (argc < 2) {
		printf("Error, put 2 args: ./editor filename\n");
		return 0;
	}
	
	file = fopen(fileName, "r");
	
	if (file == NULL) {
		printf("Error, file not found\n");
		return 0;
	}
	
	initCurses();
	initWindow(&editor, NROWS, NCOLS, y, x);
	
	while (fgets(buf, BUF_LENGTH, file) != '\0') {
		mvwprintw(editor.editWindow, line, x, "%s", buf);
		line++;
	}	
	
	while (ch != KEY_F(1)) {
		ch = wgetch(editor.editWindow);
		getyx(editor.editWindow, curY, curX);
		switch(ch) {
			case KEY_UP:
				wmove(editor.editWindow, --curY, curX);
				break;
			case KEY_DOWN:
				wmove(editor.editWindow, ++curY, curX);
				break;
			case KEY_RIGHT:
				wmove(editor.editWindow, curY, ++curX);
				break;
			case KEY_LEFT:
				wmove(editor.editWindow, curY, --curX);
				break;
			case 263:
				mvwdelch(editor.editWindow, curY, --curX);
				break;
			default:
				mvwprintw(editor.editWindow, curY, curX, "%c", ch);
				break;
		}
		wrefresh(editor.editWindow);
	}
	fclose(file);
	file = fopen(fileName, "w");
	for (i = 0; i < NROWS; i++) {
		mvwinstr(editor.editWindow, i, 0, buf);
		fprintf(file, "%s\n", buf);
	}
	endwin();
	fclose(file);
	
	return 0;
}
