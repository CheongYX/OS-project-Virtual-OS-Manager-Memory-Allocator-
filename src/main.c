#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

extern PhysicalFrame physical_mem[PHYSICAL_FRAMES];
extern FIFOQueue replacement_queue;
extern PageStats stats;

void initialize_memory() {
    memset(physical_mem, 0, sizeof(physical_mem));
    for (int i = 0; i < PHYSICAL_FRAMES; i++) {
        physical_mem[i].page_id = -1;
        physical_mem[i].is_allocated = 0;
        physical_mem[i].last_accessed = -1;
    }
    fifo_init(&replacement_queue, PHYSICAL_FRAMES);
    
    if (isatty(fileno(stdout))) { 
        printf("Memory initialized with %d frames\n", PHYSICAL_FRAMES);
    }
}

void print_json_status() {
    setvbuf(stdout, NULL, _IONBF, 0);
    
    printf("{\n");
    printf("  \"physical_memory\": [\n");
    for (int i = 0; i < PHYSICAL_FRAMES; i++) {
        printf("    {\"frame\": %d, \"page\": %d, \"allocated\": %d}%s\n",
              i,
              physical_mem[i].page_id,
              physical_mem[i].is_allocated,
              i < PHYSICAL_FRAMES-1 ? "," : "");
    }
    printf("  ],\n");
    
    printf("  \"fifo_queue\": [");
    for (int i = 0; i < replacement_queue.size; i++) {
        int idx = (replacement_queue.head + i) % replacement_queue.capacity;
        printf("%d%s", replacement_queue.frames[idx], 
              i < replacement_queue.size-1 ? ", " : "");
    }
    printf("],\n");
    
    printf("  \"stats\": {\n");
    printf("    \"page_faults\": %d,\n", stats.page_faults);
    printf("    \"hit_rate\": %.2f\n", get_hit_rate());
    printf("  }\n");
    printf("}\n");
}

int main(int argc, char *argv[]) {
    initialize_memory();

    int json_mode = 0;
    if (argc > 1 && strcmp(argv[1], "--json") == 0) {
        json_mode = 1;
    }

    int test_sequence[] = {1, 3, 0, 3, 5, 6, 3, 1, 0, 2};
    int seq_len = sizeof(test_sequence)/sizeof(int);

    for (int i = 0; i < seq_len; i++) {
        int page = test_sequence[i];
        if (is_page_in_memory(page)) {
            stats.page_hits++;
            if (!json_mode) printf("Page %d: Hit\n", page);
        } else {
            stats.page_faults++;
            if (!json_mode) printf("Page %d: Fault\n", page);
            handle_page_fault(page);
        }
    }

    if (json_mode) {
        print_json_status();
    } else {
        printf("\nTotal Page Faults: %d\n", stats.page_faults);
        printf("Total Page Hits: %d\n", stats.page_hits);
        printf("Hit Rate: %.2f%%\n", get_hit_rate() * 100);
    }

    return 0;
}