#include "../include/malloc.h"
#define CHUNK_SIZE sizeof(Chunk)

Chunk *head = NULL;

void init_hook(void) {
        __malloc_hook = m_malloc_hook;
        __free_hook = m_free_hook;
}

void *m_malloc_hook(size_t size, const void *caller) {
	void *ptr;
	Chunk *chunk = NULL, *last = NULL;
	size = align(size);
	if (head == NULL) {
		chunk = extend_heap(last, size);
		if (chunk == NULL)
			return NULL;
		head = chunk;
	} else {
		chunk = find_chunk(&last, size);
		if (!chunk) {
			chunk = extend_heap(last, size);
			if (!chunk)
				return NULL;
		}
	}
	ptr = sbrk(0);
        printf("allocate memory: \n%p size = %d\nnext addr: %p\n",
			(void *) chunk, (int)chunk->size, (void *) ptr);

        return (void *)(chunk + CHUNK_SIZE);
}

void m_free_hook(void *ptr, const void *caller) {
	Chunk *list = head;
	Chunk *free_chunk = (Chunk *) ptr - CHUNK_SIZE;

	while(list != NULL) {
		if (list == free_chunk) {
			list->free = 0;
			printf("pointer with addr %p is free\n", (void *) list);
			return;
		}
		list = list->next;
	}
}

int align(int size) {
	if (size % 8 == 0)
		return size;
	else
		return (size + 8 - size % 8);
}

Chunk *find_chunk(Chunk **last, size_t size) {
	Chunk *chunk = head;
	while (chunk->size < size || chunk->free == 1) {
		*last = chunk;
		chunk = chunk->next;
		if (chunk == NULL)
			return NULL;
	}
	chunk->free = 1;
	return chunk;
}

Chunk *extend_heap(Chunk *last, size_t size) {
	Chunk *chnk;
	chnk = sbrk(0);
	if (sbrk(CHUNK_SIZE + size) == (void *)-1) {
		return NULL;
	}
	chnk->size = size;
	chnk->next = NULL;
	if (last)
		last->next = chnk;
	chnk->free = 1;
	return chnk;
}
