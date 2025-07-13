#ifndef MEMORY_H
#define MEMORY_H

#define PHYSICAL_FRAMES 4
#define PAGE_SIZE 4096

typedef struct {
    int page_id;
    int is_allocated;
} PhysicalFrame;

void initialize_memory();

#endif