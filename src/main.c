#include <stdio.h>
#include "memory.h"

void initialize_memory() {
    PhysicalFrame phys_mem[PHYSICAL_FRAMES] = {0};
    printf("Memory initialized with %d frames\n", PHYSICAL_FRAMES);
}

int main() {
    initialize_memory();
    return 0;
}