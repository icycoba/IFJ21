CLAGS=-std=c99 -Wall -Werror -Wextra -pedantic -g
BIN=ifj21
CC=gcc

all: error.o symtable.o
	$(CC) $(CFLAGS) -o $(BIN) error.o symtable.o

clean:
	rm error.o symtable.o $(BIN)