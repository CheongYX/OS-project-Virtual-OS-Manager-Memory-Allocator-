#ifndef MEMORY_H
#define MEMORY_H
#include <stdbool.h>
#define PHYSICAL_FRAMES 4
#define PAGE_SIZE 4096
#define INVALID_PAGE -1
typedef struct {
    int page_id;
    bool is_allocated;
    int last_accessed;
} PhysicalFrame;

typedef struct {
    int* frames;
    int head, tail, size, capacity;
} FIFOQueue;
typedef struct{
    int page_faults;
    int page_hits;
}PageStats;

void initialize_memory(void);
void fifo_init(FIFOQueue* queue, int capacity);
void fifo_enqueue(FIFOQueue* queue, int frame);
int fifo_dequeue(FIFOQueue* queue);
// void fifo_display(FIFOQueue* queue); 

int get_fault_count(void);
float get_hit_rate(void);
void simulate_accesses(void);

bool is_page_in_memory(int page_id);
bool is_page_in_queue(FIFOQueue* queue, int page_id); 
void handle_page_fault(int page_id);

extern PhysicalFrame physical_mem[PHYSICAL_FRAMES];
extern FIFOQueue replacement_queue;
extern PageStats stats;

#endif