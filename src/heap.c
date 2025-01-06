#include "../include/heap.h"

void *heap = NULL;

int init_heap() {
    heap = mmap(NULL, INIT_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (heap == MAP_FAILED) {
        perror("mmap failed during heap initalization");
        return -1;
    }

    // Split our heap into blocks
    struct block_metadata *initial_block = (struct block_metadata *)heap;   // 1 big free block
    initial_block->size = INIT_HEAP_SIZE - BLOCK_SIZE;  // INIT_HEAP_SIZE - BLOCK_SIZE = available space for user data
    initial_block->free = 1;    // list all that user space as free
    initial_block->next = NULL; // next block is null

    return 0;
}

int destroy_heap() {
    if(munmap(heap, INIT_HEAP_SIZE)) {
        perror("munmap");
        return -1;
    }
    return 0;
}

size_t align_size(size_t size) {
    // size + BLOCK_SIZE - 1 ensures size is a multiple if it wasnt already
    // bitwise AND clears lower bit that represents remainder, rounding up to a multiple
    return (size + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);
}

void *my_memset(void *ptr, int value, size_t num) {
    unsigned char *p = ptr;
    while (num--) {
        *p = (unsigned char) value;
        p++;
    }
    return ptr;
}

void *my_memcpy(void *dest, const void *src, size_t n) {
    // Cast to char * for byte-wise copying
    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
}

void *my_malloc(size_t size) {
    struct block_metadata *block = heap;
    struct block_metadata *last = NULL;

    if (size <= 0) {
        return NULL;
    }

    size = align_size(size);    // Align size

    // terminates upon finding suitable block or end of memory block list
    while (block && !(block->free && block->size >= size)) {
        last = block;
        block = block->next;
    }

    if (!block) {   // valid block not found
        fprintf(stderr, "Error: Unable to allocate memory block of size %ld\n", size);
        return NULL;
    }

    // Coalescing, if curr block is larger than size + metadata split block
    if (block->size > size + BLOCK_SIZE ) {
        /**
         * BASICALLY we found a curr block that is for userspace
         * we split this current block via new block and giving it the size of our new user space
         * set its size to the space after allocation, mark it as free, and update its next
         * set curr blocks size, and point it to new block (new block being our new available user space)
         */
        struct block_metadata *new_block = (struct block_metadata *)((char *)block + BLOCK_SIZE + size);
        new_block->size = block->size - size - BLOCK_SIZE;  // size of leftover space
        new_block->free = 1;            // marking as free
        new_block->next = block->next;  // link to next

        block->size = size;         // shrink the current block to requested size
        block->next = new_block;    // point to the new block
    }

    // Mark as no longer being free, and return
    block->free = 0;
    return (char *)block + BLOCK_SIZE;  // return size of block + block_size to accomadate user size + metadata
}

void *my_calloc(size_t nelem, size_t elsize) {
    struct block_metadata *block = heap;
    struct block_metadata *last = NULL;
    size_t total_size = nelem * elsize;

    if (total_size <= 0) {
        fprintf(stderr, "Error: Unable to allocate memory block of size %ld\n", total_size);
        return NULL;
    }
    total_size = align_size(total_size);

    // terminates upon finding suitable block or end of memory block list
    while (block && !(block->free && block->size >= total_size)) {
        last = block;
        block = block->next;
    }

    if (!block) {   // valid block not found
        fprintf(stderr, "Error: Unable to allocate memory block of size %ld\n", total_size);
        return NULL;
    }

    // Coalescing, if curr block is larger than size + metadata split block
    if (block->size > total_size + BLOCK_SIZE ) {
        /**
         * BASICALLY we found a curr block that is for userspace
         * we split this current block via new block and giving it the size of our new user space
         * set its size to the space after allocation, mark it as free, and update its next
         * set curr blocks size, and point it to new block (new block being our new available user space)
         */
        struct block_metadata *new_block = (struct block_metadata *)((char *)block + BLOCK_SIZE + total_size);
        new_block->size = block->size - total_size - BLOCK_SIZE;  // size of leftover space
        new_block->free = 1;            // marking as free
        new_block->next = block->next;  // link to next

        block->size = total_size;         // shrink the current block to requested size
        block->next = new_block;    // point to the new block
    }

    // Mark as no longer being free, and return
    my_memset((char *)block + BLOCK_SIZE, 0, total_size);
    block->free = 0;
    return (char *)block + BLOCK_SIZE;  // return size of block + block_size to accomadate user size + metadata
}

void *my_realloc(void *ptr, size_t size) {
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    if (!ptr) {
        return my_malloc(size);  // If ptr is NULL, behave like malloc
    }

    size = align_size(size); 
    struct block_metadata *block = (struct block_metadata *)((char *)ptr - BLOCK_SIZE);
    
    // If the current block size is equal or larger to new size
    if (block->size >= size) {
        return ptr;
    }

    // Allocate a new block
    void *new_ptr = my_malloc(size);
    if (!new_ptr) {
        return NULL;
    }

    // Copy the existing data to the new block
    size_t copy_size;
    if (block->size < size)
        copy_size = block->size;
    else
        copy_size = size;

    my_memcpy(new_ptr, ptr, copy_size);
    my_free(ptr);  // Free the old block

    return new_ptr;
}

void my_free(void *ptr) {
    if (!ptr) {
        return;
    }

    struct block_metadata *block = (struct block_metadata *)((char *)ptr - BLOCK_SIZE);
    block->free = 1;

    // Coalescing
    if (block->next && block->next->free) { // check if next block isnt null, and is also free
        block->size += block->next->size + BLOCK_SIZE;   // sum their sizes + metadata size
        block->next = block->next->next;
    }

}

