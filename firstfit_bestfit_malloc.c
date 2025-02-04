#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MEMORY_POOL_SIZE 1024

typedef struct MemBlock{
    size_t size;
    bool free;
    struct MemBlock* next;
} MemBlk;

static char memory_pool[MEMORY_POOL_SIZE]; 
static MemBlk* free_list = (MemBlk*)memory_pool;

void initialize_memory(){
    free_list->size = MEMORY_POOL_SIZE;
    free_list->free = true;
    free_list->next = NULL;
}

void split_block(MemBlk* fittingBlk, size_t size){
    MemBlk* new_block = (MemBlk*)((char*)fittingBlk + sizeof(MemBlk) + size);
    new_block->size = fittingBlk->size - size - sizeof(MemBlk);
    new_block->free = true;
    new_block->next = fittingBlk->next;

    fittingBlk->size = size;
    fittingBlk->free = false;
    fittingBlk->next = new_block;
}

void* first_fit_alloc(size_t size){
    MemBlk* curr = free_list;
    while(curr != NULL){
        if(curr->free && curr->size>=size){
            if(curr->size>size){
                split_block(curr, size);
            }
            else{
                curr->free = false;
            }
            return (char*)curr + sizeof(MemBlk);
        }
        curr = curr->next;
    }
    return NULL;
}

void* best_fit_alloc(size_t size){
    MemBlk* curr = free_list;
    MemBlk* bestFit = NULL;

    while(curr != NULL){
        if(curr->free && curr->size>=size){
            if(bestFit == NULL || bestFit->size > curr->size){
                bestFit = curr;
            }
        }
        curr = curr->next;    
    }

    if(bestFit != NULL){
        if(bestFit->size > size){
            split_block(bestFit, size);
        }
        else{
            bestFit->free = false;
        }
        return (char*)bestFit + sizeof(MemBlk);
    }
    return NULL;
}

void memory_free(void* ptr) {
    if (ptr == NULL) return;

    MemBlk* blkToFree = (MemBlk*)((char*)ptr - sizeof(MemBlk));
    blkToFree->free = true;

    // Iterate through free_list to merge any adjacent free blocks
    MemBlk* curr = free_list;
    while (curr != NULL && curr->next != NULL) {
        // Merge with next block if both are free
        if (curr->free && curr->next->free) {
            curr->size += sizeof(MemBlk) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

// Utility function to display the current state of memory
void display_memory() {
    MemBlk* current = free_list;
    while (current != NULL) {
        printf("Block: %p, Size: %zu, Free: %d\n", (void*)current, current->size, current->free);
        current = current->next;
    }
}

int main() {
    initialize_memory();

    printf("Initial memory state:\n");
    display_memory();

    void* ptr1 = first_fit_alloc(200);
    printf("\nAfter first-fit allocation of 200 bytes:\n");
    display_memory();

    void* ptr2 = best_fit_alloc(150);
    printf("\nAfter best-fit allocation of 150 bytes:\n");
    display_memory();

    memory_free(ptr1);
    printf("\nAfter freeing the first 200-byte block:\n");
    display_memory();

    void* ptr3 = first_fit_alloc(300);
    printf("\nAfter first-fit allocation of 300 bytes:\n");
    display_memory();

    memory_free(ptr2);
    printf("\nAfter freeing the second 150-byte block:\n");
    display_memory();

    void* ptr4 = first_fit_alloc(150);
    printf("\nAfter best-fit allocation of 150 bytes:\n");
    display_memory();

    memory_free(ptr3);
    printf("\nAfter freeing the third 300-byte block:\n");
    display_memory();

    memory_free(ptr4);
    printf("\nAfter freeing the third 150-byte block:\n");
    display_memory();

    return 0;
}
