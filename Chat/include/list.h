#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma once

typedef struct _list {
	long type;
	char name[32];
	char message[255];
	struct _list *next;
} List;

List *initList(long, char*);
List *addElem(long, char*, List*);
List *delElem(List*, List*);
List *swap(List*, List*, List*);
int sizeList(List*);
void clearList(List*);
