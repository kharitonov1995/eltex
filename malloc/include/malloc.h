#include <malloc.h>

typedef struct _chunk {
	size_t size;
	struct _chunk *next;
	int free;
} Chunk;

void init_hook(void);
void *m_malloc_hook(size_t, const void *);
void m_free_hook(void *, const void *);

int align(int);
Chunk *find_chunk(Chunk **, size_t);
Chunk *extend_heap(Chunk *, size_t);

void (*__malloc_initialize_hook) (void) = init_hook;


