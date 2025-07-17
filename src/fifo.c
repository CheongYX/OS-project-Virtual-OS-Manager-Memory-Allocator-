#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

PageStats stats = {0,0};
PhysicalFrame physical_mem[PHYSICAL_FRAMES];
FIFOQueue replacement_queue;

void fifo_init(FIFOQueue* queue, int capacity) {
    queue->frames = (int*)malloc(capacity * sizeof(int));
    if (!queue->frames) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    queue->capacity = capacity;
    queue->head = queue->tail = queue->size = 0;
}

bool is_page_in_queue(FIFOQueue* queue, int page_id) {
    for (int i = 0; i < queue->size; i++) {
        int idx = (queue->head + i) % queue->capacity;
        if (queue->frames[idx] == page_id) {
            return true;
        }
    }
    return false;
}

bool is_page_in_memory(int page_id) {
    for (int i = 0; i < PHYSICAL_FRAMES; i++) {
        if (physical_mem[i].is_allocated && 
            physical_mem[i].page_id == page_id) {
            physical_mem[i].last_accessed = stats.page_faults + stats.page_hits;
            return true;
        }
    }
    return false;
}

void fifo_enqueue(FIFOQueue* queue, int frame) {
    if (queue->size >= queue->capacity) {
        fprintf(stderr, "Error: Queue is full\n");
        return;
    }
    queue->frames[queue->tail] = frame;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;
}

int fifo_dequeue(FIFOQueue* queue) {
    if (queue->size <= 0) {
        fprintf(stderr, "Error: Queue is empty\n");
        return -1;
    }
    int frame = queue->frames[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;
    return frame;
}

void handle_page_fault(int page_id) {
    for (int i = 0; i < PHYSICAL_FRAMES; i++) {
        if (!physical_mem[i].is_allocated) {
            physical_mem[i].page_id = page_id;
            physical_mem[i].is_allocated = 1;
            fifo_enqueue(&replacement_queue, i);
            return;
        }
    }
    int victim_idx = fifo_dequeue(&replacement_queue);
    physical_mem[victim_idx].page_id = page_id;
    fifo_enqueue(&replacement_queue, victim_idx);
}

int get_fault_count(void){
    return stats.page_faults;
}

float get_hit_rate(void){
    int total_accesses = stats.page_faults+stats.page_hits;
    return (total_accesses == 0) ? 0.0f : (float)stats.page_hits / total_accesses;
}
