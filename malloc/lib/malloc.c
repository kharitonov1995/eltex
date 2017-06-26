#include <stdlib.h>
#include <stdio.h>

#include "../include/malloc.h"

void init_hook(void) {
        __malloc_hook = m_malloc_hook;
        __free_hook = m_free_hook;
}

void *m_malloc_hook(size_t size, const void *caller) {
        void *ptr, *b;
        ptr = sbrk(0);
        sbrk(size);
        b = sbrk(0);
        printf("a = %x\nb = %x\n", ptr, b);

        return ptr;
}

void m_free_hook(void *ptr, const void *caller) {
        printf("321\n");
        free(ptr);
}
