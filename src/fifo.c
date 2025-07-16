#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

void fifo_init(FIFOQueue* queue, int capacity) {
    queue->frames = (int*)malloc(capacity * sizeof(int));
    queue->capacity = capacity;
    queue->head = queue->tail = queue->size = 0;
}

void fifo_enqueue(FIFOQueue* queue, int frame) {
    if (queue->size >= queue->capacity) {
        printf("Error: Queue is full\n");
        return;
    }
    queue->frames[queue->tail] = frame;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;
}

int fifo_dequeue(FIFOQueue* queue) {
    if (queue->size <= 0) {
        printf("Error: Queue is empty\n");
        return -1;
    }
    int frame = queue->frames[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;
    return frame;
}