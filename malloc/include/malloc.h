#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

void init_hook(void);
void *m_malloc_hook(size_t, const void *);
void m_free_hook(void *, const void *);

void (*__malloc_initialize_hook) (void) = init_hook;
