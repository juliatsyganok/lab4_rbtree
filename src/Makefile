CC = gcc
CFLAGS = -Wall -Wextra -g

all: program

program: main.o rbtree.o
	$(CC) $(CFLAGS) -o program main.o rbtree.o

main.o: main.c rbtree.h
	$(CC) $(CFLAGS) -c main.c

rbtree.o: rbtree.c rbtree.h
	$(CC) $(CFLAGS) -c rbtree.c

clean:
	rm -f *.o program output.txt

run: program
	./program

.PHONY: all clean run