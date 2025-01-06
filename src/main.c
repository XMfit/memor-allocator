#include "../include/heap.h"

int main() {

    init_heap();
    printf("Location %p\n", (void *)heap);

    int *ptr1 = (int *)my_malloc(sizeof(int));
    *ptr1 = 1;
    printf("Location %p Value: %d\n", (void *)ptr1, *ptr1);
    
    char *ptr2 = (char *)my_malloc(sizeof(char));
    *ptr2 = 'A';
    printf("Location %p Value: %c\n", (void *)ptr2, *ptr2);
    
    char *str = (char *)my_malloc(sizeof(char) * 3);
    str[0] = 'H';
    str[1] = 'i';
    str[2] = '\0';
    printf("Location %p Value: %s\n", (void *)str, str);

    double *ptr3 = (double *)my_malloc(sizeof(double));
    *ptr3 = 3.14159f;
    printf("Location %p Value: %f\n", (void *)ptr3, *ptr3);

    my_free(ptr2);
    my_free(ptr1);
    my_free(str);
    my_free(ptr3);

    int size = 10;
    int *ptr4 = my_calloc(size, sizeof(int));
    for (int i = 0; i < size; i++) {
        printf("%d\n", ptr4[i]);
    }

    int *ptr5 = my_malloc(sizeof(int));
    *ptr5 = 2;
    printf("Location %p Value: %d\n", (void *)ptr5, *ptr5);

    ptr4 = my_realloc(ptr4, size / 2);
    for (int i = 0; i < size / 2; i++) {
        ptr4[i] = i;
        printf("%d\n", ptr4[i]);
    }

    destroy_heap();
    return 0;
}

