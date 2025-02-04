#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define HEAP_SIZE 1024

static char heap[HEAP_SIZE];
static char *heap_current = heap;

void *heap_start() {
    return heap;
}

void *heap_end() {
    return heap + HEAP_SIZE;
}

void *sbrk(intptr_t increment) {
    if ((void *)(heap_current + increment) > (void *)heap_end()) {
        return (void *)-1;
    }
    void *previous_position = heap_current;
    heap_current += increment;
    return previous_position;
}

void *find_fit(size_t size){
    size_t *header = (size_t *)heap_start();
    while ((char *)header < (char *)heap_current) {
        size_t block_size = *header & ~1L;
        int allocated = *header & 1;
        
        printf("Inspecting block at %p, size: %lu, allocated: %lu\n", header, block_size, (size_t)allocated);
        
        if (!allocated && block_size >= size) {
            return header;
        }
        header = (size_t *)((char *)header + block_size);
    }
    return NULL;
}

void *malloc(size_t size){
    size_t blk_size = ALIGN(size + SIZE_T_SIZE);
    size_t *header = find_fit(blk_size);
    
    if (!header) {
        header = (size_t *)sbrk(blk_size);
        if (header == (void *)-1) {
            return NULL;
        }
        *header = blk_size | 1;  // Mark block as allocated
    } else {
        *header |= 1;  // Mark found block as allocated
    }
    
    return (char *)header + SIZE_T_SIZE;
}

void free(void *ptr) {
    if (!ptr) return;
    size_t *header = (size_t *)((char *)ptr - SIZE_T_SIZE);
    *header &= ~1L;  // Mark block as free
}

int main() {
    printf("Starting custom malloc/free test...\n");

    char *ptr1 = (char *)malloc(32);
    if (ptr1) {
        strcpy(ptr1, "Hello, Heap!");
        printf("Allocated and stored string in 32-byte block: %s\n", ptr1);
    }

    char *ptr2 = (char *)malloc(64);
    if (ptr2) {
        strcpy(ptr2, "Testing custom malloc implementation");
        printf("Allocated and stored string in 64-byte block: %s\n", ptr2);
    }

    free(ptr1);
    printf("Freed the first 32-byte block.\n");

    char *ptr3 = (char *)malloc(16);
    if (ptr3) {
        strcpy(ptr3, "Tiny block");
        printf("Allocated and stored string in 16-byte block: %s\n", ptr3);
    }

    free(ptr2);
    free(ptr3);
    printf("Freed all allocated blocks.\n");

    return 0;
}
