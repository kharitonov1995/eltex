#include "../include/list.h"

List *initList(char *string) {
	List *list;
	
	list = (List*)malloc(sizeof(List));
	list->data = string;
	list->next = NULL;
	
	return list;
}

List *addElem(char *string, List *list) {
	List *p, *temp;
	
	temp = (List*)malloc(sizeof(List));
	p = list->next;
	list->next = temp;
	temp->data = string;
	temp->next = p;
	
	return temp;
}

List *delElem(List *list, List *root) {
	List *temp;
	
	temp = root;
	while (temp->next != list) {
		temp = temp->next;
	}
	temp->next = list->next;
	free(list);
	
	return temp;
}

int sizeList(List *head) {
	List *list;
	int size = 0;
	
	if (head->next == NULL)
		return 1;
	else {
		list = head;
		do {
			size++;
			list = list->next;
		} while (list != NULL);
		return size;
	}
}

void clearList(List *head) {
	List *current = head;
	List *next;
 
	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
}
