#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Required for sbrk


#define ALIGNMENT 8
#define ALIGN(size) (((size + (ALIGNMENT-1)) & ~(ALIGNMENT-1)))
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

void *malloc(size_t size){
    size_t blk_size = ALIGN(size + SIZE_T_SIZE);
    size_t *header = sbrk(blk_size);
    if (header == (void *)-1) {
        return NULL; // sbrk failed
    }
    *header = blk_size | 1;
    return (char *)header + SIZE_T_SIZE;
}

void free(void *ptr){
    if (!ptr) return;
    size_t *header = (char *)ptr - SIZE_T_SIZE;
    *header = *header & ~1L;
}

int main() {
    // Test case 1: Allocate 20 bytes and write something
    char *ptr1 = (char *)malloc(20);
    if (ptr1 == NULL) {
        printf("malloc failed\n");
        return 1;
    }
    strcpy(ptr1, "Hello, World!");
    printf("Allocated string: %s\n", ptr1);

    // Test case 2: Allocate 40 bytes
    char *ptr2 = (char *)malloc(40);
    if (ptr2 == NULL) {
        printf("malloc failed\n");
        return 1;
    }
    strcpy(ptr2, "Testing custom malloc");
    printf("Allocated string: %s\n", ptr2);

    // Free both allocated blocks
    free(ptr1);
    free(ptr2);

    return 0;
}
