#ifndef HEAP_H
#define HEAP_H

/* Needed Libaries */
#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

/* Size Macros */
#define INIT_HEAP_SIZE (1 << 25)                              // Size of heap 2MB
#define BLOCK_SIZE sizeof(struct block_metadata)              // Size of a block

/* Heap Pointer */
extern void *heap;

/* Memory metadata structure */
struct block_metadata {
    size_t size;                    // size of block
    int free;                       // flag for if its free or not
    struct block_metadata *next;    // points to next block
};

/* Functions to manage heap structure */
int init_heap();
int destroy_heap();

/* Expected functions to aid in malloc() and free()*/
size_t align_size(size_t size);
void *my_memset(void *ptr, int value, size_t num);
void *my_memcpy(void *dest, const void *src, size_t n);

/* Functions for dynamic allocation */
void *my_malloc(size_t size);
void *my_calloc(size_t nelem, size_t elsize);
void *my_realloc(void *ptr, size_t size);
void my_free(void *ptr);

#endif
