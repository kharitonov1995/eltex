#include <stdlib.h>

typedef struct _list {
	char *data;
	struct _list *next;
} List;

List *initList(char*);
List *addElem(char*, List*);
List *delElem(List*, List*);
