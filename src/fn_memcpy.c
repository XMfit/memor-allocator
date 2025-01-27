#include "../include/heap.h"

void fn_memcpy(void *dest, const void *src, size_t n) {
    // Cast to char * for byte-wise copying
    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
}

