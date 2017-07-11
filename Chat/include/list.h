#include <stdlib.h>

#pragma once

typedef struct _list {
	long type;
	char *name;
	struct _list *next;
} List;

List *initList(long, char*);
List *addElem(long, char*, List*);
List *delElem(List*, List*);
List *swap(List*, List*, List*);
int sizeList(List*);
void clearList(List*);
