#include <string.h>
#include "../include/list.h"

List *initList(long type, char *string) {
	List *list;
	
	list = malloc(sizeof(List));
	list->type = type;
	memcpy(list->name, string, strlen(string));
	list->next = NULL;
	
	return list;
}

List *addElem(long type, char *string, List *list) {
	List *p, *temp;
	
	temp = malloc(sizeof(List));
	p = list->next;
	list->next = temp;
	memcpy(temp->name, string, strlen(string));
	temp->type = type;
	temp->next = p;
	
	return temp;
}

List *delElem(List *elem, List *head) {
	 List *temp, *prev;
	 
    if(head == elem) {
        if(head->next == NULL) {
            free(head);
            return NULL;
        }
        
        head->type = head->next->type;
        memcpy(head->name, head->next->name, strlen(head->next->name));
		
        elem = head->next;
 
        head->next = head->next->next;
 
        free(elem);
 
        return head;
    }
 
	prev = head;
    while(prev->next != NULL && prev->next != elem)
        prev = prev->next;
 
    if(prev->next == NULL) {
        return head;
    }
 
    prev->next = prev->next->next;
    free(elem);
 
    return head;
}

List *searchElem(long type, List *head) {
	List *list = NULL;
	
	list = head;
	while (list != NULL) {
		if (list->type == type) {
			return list;
		}
		list = list->next;
	}
	return NULL;
}

/* Возвращает новый корень списка */
List *swap(List *head, List *list1, List *list2) {
	List *prev1, *prev2, *next1, *next2;
	
	prev1 = head;
	prev2 = head;
	
	if (prev1 == list1)
		prev1 = NULL;
	else
		while (prev1->next != list1)
		  prev1 = prev1->next;
	if (prev2 == list2)
		prev2 = NULL;
	else
		while (prev2->next != list2)
		  prev2 = prev2->next;
	
	next1 = list1->next;
	next2 = list2->next;
	
	if (list2 == next1) {
		list2->next = list1;
		list1->next = next2;
		if (list1 != head)
			prev1->next = list2;
	} else {
		if (list1 == next2) {
			list1->next = list2;
			list2->next = next1;
			if (list2 != head)
				prev2->next = list2;
		} else {
			if (list1 != head)
				prev1->next = list2;
			list2->next = next1;
			if (list2 != head)
				prev2->next = list1;
			list1->next = next2;
		}
	}
	
	if (list1 == head)
		return(list2);
	if (list2 == head)
		return(list1);
	return(head);
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
