CC=gcc
CFLAGS=-I./include
BIN=bin/vm_manager

all:
    $(CC) $(CFLAGS) src/main.c -o $(BIN)

run:
    ./$(BIN)

clean:
    rm -f $(BIN)