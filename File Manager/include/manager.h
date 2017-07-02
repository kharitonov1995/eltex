/*! \mainpage Some description of project File Manager
 *
 * \section intro_sec Introduction
 *
 * Надо было сделать файловый менеджер, а получилось это....
 */
#include <curses.h>
#include <panel.h>

#include "../include/list.h"

const int MAX_PATH = 128; /**< Is maximum length of absolute path. */
const int LENGTH_NAME = 255; /**< Is length name of elements in menu. */
const int SIZE_COPY_SINDOW = 32;/**< Is size window which showing proccess copying, also it is size of block of copied data. */

/**
 * @brief Struct panel
 * 
 * Using for store information about each of panels.
 * 
 **/
typedef struct _panel {
	char path[128]; /**< Is absolute path to current directory. */
	WINDOW *windowMenu; /**< Window which shows all information. */
	char **items; /**< List of all items for current directory. */
	int startX; /**< Coordinate on x. */
	int startY; /**< Coordinate on y. */
	int countItems; /**< Count all elements on current directory. */
	int beginPos; /**< Starting position in the list for page. */
	int endPos; /**< Ending position in the list for page. */
	int selectItem; /**< Position selectable item. */
} panel;

/**
 * @brief Struct of arguments
 * 
 * Using for store and sending argumets to threads.
 * 
 **/
struct argsThread {
	char *targetPath; /**< Is absolute path to target file. */
	char *sourcePath; /**< Is absolute path to source file. */
	FILE *file; /**< Is file which needed copy. */
	WINDOW *win; /**< Is window which showing field of input for target file. */
};

/**
 * initCurses() initialize all properties for terminal.
 */ 
void initCurses();

/**
 * initPanel(panel*, int, int) initialize all properties for one panel.
 * @param panel* is address of initialize panel.
 * @param int is start coordinate on Y.
 * @param int is start coordinate on X.
 */ 
void initPanel(panel*, int, int);

/**
 * initWindowInfo() initialize window which showing navigation bar.
 * @return created window.
 */ 
WINDOW *initWindowInfo();

/**
 * initPanels(panel*, int, int, int) initialize all properties for multiply panel.
 * @param panel* is address of initialize panel.
 * @param int is start coordinate on Y.
 * @param int is start coordinate on X.
 * @param int is count initialize panels.
 */
void initPanels(panel*, int, int, int);

/**
 * getMaxShowLines(panel*) getting max Y for panel#windowMenu.
 * @param panel* is address of panel.
 * @return max Y for panel#windowMenu.
 */
int getMaxShowLines(panel*); 

/**
 * drawMenuPanel(panel*, int, int, int, char**) drawing menu on the panel*.
 * @param panel* is address of panel.
 * @param int is start coordinate on Y.
 * @param int is start coordinate on X.
 * @param int is mark of select item for panel*.
 * @param char** is array of strings lenght LENGTH_NAME bytes each 
 */
void drawMenuPanel(panel*, int, int, int, char**);

/**
 * printToWindow(WINDOW*, char*, int, int, int) print char* on the WINDOW*.
 * @param WINDOW* is window on which needed showing char*.
 * @param char* is string or character which needed print.
 * @param int is start coordinate on Y.
 * @param int is start coordinate on X.
 * @param int is color.
 * @param int is attributes for text, if == 0 then attributes is off;
 */
void printToWindow(WINDOW*, char*, int, int, int, int);

/**
 * destructPanel(panel*) free items menu of panel and destruct panel.
 * @param panel is panel which needed dectruct.
 */
void destructPanel(panel*);

/**
 * execFile(char*, char*, int) running file.
 * @param char* is path to file.
 * @param char* is filename.
 * @param int is flag which means an executable or text file.
 */
void execFile(char*, char*, int);

/**
 * getCountFilesDir(char*) getting count elements of directory.
 * @param char* is path to directory.
 */
int getCountFilesDir(char*);

/**
 * getFilesDir(panel*) getting all files for panel*.
 * @param panel* is panel which must be filled with elements.
 * @return count elements of directory
 */
void getFilesDir(panel*);

/**
 * changeDirectory(char*, char*) changing directory.
 * @param char* is absolute path to source directory.
 * @param char* is name dectionary directory.
 */
void changeDirectory(char*, char*);

/**
 * isDirectory(char*) checking char* for directory.
 * @param char* is name directory.
 * @return 1 if char* is directory, otherwise 0.
 */
int isDirectory(char*);

/**
 * isExecFile(char*) checking char* for executable file.
 * @param char* it's name file.
 * @return 1 if char* is executable file, otherwise 0.
 */
int isExecFile(char*);

/**
 * threadCopy(void*) is function of thread copy.
 * @param void* is pointer on struct argsThread.
 */
void *threadCopy(void*);

/**
 * threadDraw(void*) is function of thread draw.
 * @param void* is pointer on struct argsThread.
 */
void *threadDraw(void*);

/**
 * copyForm(panel*, char*) is function which build window copy and showing copy procces.
 * @param panel* it's address of panel.
 * @param char* is file name which must be copied.
 */
void copyForm(panel*, char*);

/**
 * trimSpaces(char*) is function which deleting spaces at end of string.
 * @param char* is string which must be trimmed.
 * @return pointer on the string without spaces.
 */
char *trimSpaces(char*);
