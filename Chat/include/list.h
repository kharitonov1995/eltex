#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma once

typedef struct _list {
	long type;
	struct _list *next;
	char name[32];
} List;

List *initList(long, char*);
List *addElem(long, char*, List*);
List *searchElem(long, List*);
void delElem(List*, List**);
List *swap(List*, List*, List*);
int sizeList(List*);
void clearList(List*);
