#include <stdlib.h>
#include <stdio.h>

#include "../include/malloc.h"
#define CHUNK_SIZE sizeof(Chunk)

Chunk *head = NULL;

void init_hook(void) {
        __malloc_hook = m_malloc_hook;
        __free_hook = m_free_hook;
}

void *m_malloc_hook(size_t size, const void *caller) {
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
        printf("allocate memory: \n0x%x %d\n", chunk, chunk->size);
        return (void *)(chunk + CHUNK_SIZE);
}

void m_free_hook(void *ptr, const void *caller) {
        printf("321\n");
        free(ptr);
}

int align(int size) {
	if (size % 8 == 0)
		return size;
	else {
		return (size + 8 - size % 8);
	}
}

Chunk *find_chunk(Chunk **last, size_t size) {
	Chunk *chunk = head;
	while (chunk != NULL && (chunk->size >= size && chunk->free == 0)) {
		*last = chunk;
		chunk = chunk->next;
	}
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
	chnk->free = 0;
	return chnk;
}
