#include <stdlib.h>

#pragma once

typedef struct _list {
	char *data;
	struct _list *next;
} List;

List *initList(char*);
List *addElem(char*, List*);
List *delElem(List*, List*);
List *swap(List*, List*, List*);
int sizeList(List*);
void clearList(List*);
