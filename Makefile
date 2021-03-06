CLAGS=-std=c99 -Wall -Werror -Wextra -pedantic -g
BIN=ifj21
CC=gcc

all: error.o str.o list.o ilist.o codegen.o symtable.o parser.o scanner.o main.o
	$(CC) $(CFLAGS) -o $(BIN) error.o str.o list.o ilist.o codegen.o symtable.o parser.o scanner.o main.o

clean:
	rm error.o str.o list.o ilist.o codegen.o symtable.o parser.o scanner.o main.o  $(BIN)
