#include "../include/memory.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>


void test_fifo_operations() {
    stats.page_faults = 0;
    stats.page_hits = 0;
    fifo_init(&replacement_queue, PHYSICAL_FRAMES);

    FIFOQueue q;
    fifo_init(&q, 3);
    
    fifo_enqueue(&q, 1);
    assert(q.size == 1);
    assert(q.frames[q.head] == 1);
    
    int val = fifo_dequeue(&q);
    assert(val == 1);
    assert(q.size == 0);
    
    fifo_enqueue(&q, 2);
    fifo_enqueue(&q, 3);
    fifo_enqueue(&q, 4);
    assert(q.size == 3);

    fifo_dequeue(&q);
    fifo_dequeue(&q);
    fifo_dequeue(&q);
    assert(q.size == 0);
    assert(fifo_dequeue(&q) == -1);  

    free(q.frames);
    free(replacement_queue.frames);

    printf("FIFO tests passed!\n");
}

int main() {
    test_fifo_operations();
    return 0;
}