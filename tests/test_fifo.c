#include "../include/memory.h"
#include <assert.h>
#include <stdio.h>

void test_fifo_operations() {
    FIFOQueue q;
    fifo_init(&q, 3);
    
    fifo_enqueue(&q, 1);
    assert(q.size == 1);
    
    int val = fifo_dequeue(&q);
    assert(val == 1);
    assert(q.size == 0);
    
    printf("FIFO tests passed!\n");
}

int main() {
    test_fifo_operations();
    return 0;
}