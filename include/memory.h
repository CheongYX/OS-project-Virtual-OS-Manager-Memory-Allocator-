#ifndef MEMORY_H
#define MEMORY_H

#define PHYSICAL_FRAMES 4
#define PAGE_SIZE 4096

typedef struct {
    int page_id;
    int is_allocated;
    int last_accessed;
} PhysicalFrame;

typedef struct {
    int* frames;
    int head, tail, size, capacity;
} FIFOQueue;

void initialize_memory();
void fifo_init(FIFOQueue* queue, int capacity);
// void fifo_enqueue(FIFOQueue* queue, int frame);
// int fifo_dequeue(FIFOQueue* queue);
// void fifo_display(FIFOQueue* queue); 

extern PhysicalFrame physical_mem[PHYSICAL_FRAMES];
extern FIFOQueue replacement_queue;

#endif