#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HEAP_SIZE 128  // Total blocks of 1KB each
#define BLOCK_SIZE 1024  // Each block is 1KB

uint8_t heap[HEAP_SIZE * BLOCK_SIZE];
int block_map[HEAP_SIZE] = {0};  // 0 = free, 1 = allocated
int last_allocated_block = -1;
int last_freed_block = -1;

void* allocate_mem(size_t size) {
    int blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int best_fit_index = -1;
    int best_fit_size = HEAP_SIZE + 1;

    for (int i = 0; i < HEAP_SIZE;) {
        if (block_map[i] == 0) {
            int block_count = 0;
            while (i + block_count < HEAP_SIZE && block_map[i + block_count] == 0) {
                block_count++;
                if (block_count >= blocks_needed)
                    break;
            }

            if (block_count >= blocks_needed && block_count < best_fit_size) {
                best_fit_size = block_count;
                best_fit_index = i;
            }
            i += block_count;
        } else {
            i++;
        }
    }

    if (best_fit_index == -1 || best_fit_index == last_allocated_block)
        return NULL;

    for (int i = 0; i < blocks_needed; i++)
        block_map[best_fit_index + i] = 1;

    last_allocated_block = best_fit_index;

    return &heap[best_fit_index * BLOCK_SIZE];
}

void deallocate_mem(void* ptr) {
    int index = ((uint8_t*)ptr - heap) / BLOCK_SIZE;

    if (index == last_freed_block || block_map[index] == 0)
        return;

    for (int i = index; i < HEAP_SIZE && block_map[i] == 1; i++)
        block_map[i] = 0;

    last_freed_block = index;
}

int main() {
    void* ptr1 = allocate_mem(sizeof(int) * 128);
    void* ptr2 = allocate_mem(sizeof(uint8_t) * 1000);
    void* ptr3 = allocate_mem(128 * 8 * 1024);
    
    deallocate_mem(ptr1);
    deallocate_mem(ptr2);
    deallocate_mem(ptr3);

    return 0;
}


