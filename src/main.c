#include <stdio.h>
#include "memory.h"

PhysicalFrame physical_mem[PHYSICAL_FRAMES];
FIFOQueue replacement_queue;


void initialize_memory() {
    for (int i = 0; i < PHYSICAL_FRAMES; i++) {
        physical_mem[i].is_allocated = 0;
        physical_mem[i].last_accessed = -1;
    }
    
    fifo_init(&replacement_queue, PHYSICAL_FRAMES);
    printf("Memory initialized with %d frames\n", PHYSICAL_FRAMES);  // 注意添加%d
}

int main() {
    initialize_memory();
    return 0;
}