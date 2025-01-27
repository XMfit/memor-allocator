#include "../include/heap.h"

size_t fn_align_size(size_t size) {
    // size + BLOCK_SIZE - 1 ensures size is a multiple if it wasnt already
    // bitwise AND clears lower bit that represents remainder, rounding up to a multiple
    return (size + METADATA_SIZE - 1) & ~(METADATA_SIZE - 1);
}

