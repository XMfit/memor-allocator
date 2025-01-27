#include "../include/heap.h"

void *fn_memset(void *ptr, int value, size_t num) {
    unsigned char *p = ptr;
    while (num--) {
        *p = (unsigned char) value;
        p++;
    }
    return ptr;
}

