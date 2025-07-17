CC = gcc
CFLAGS = -Wall -Iinclude
BIN = bin/vm_manager
TEST_BIN = bin/test_fifo

SRC = src/main.c src/fifo.c
TEST_SRC = tests/test_fifo.c src/fifo.c
DEPS = include/memory.h

.PHONY: all test clean run

all: $(BIN)

$(BIN): $(SRC) $(DEPS)
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(DEPS)
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf bin

bin:
	mkdir -p bin

test_fifo: bin
	gcc -Wall -Iinclude src/fifo.c tests/test_fifo.c -o bin/test_fifo