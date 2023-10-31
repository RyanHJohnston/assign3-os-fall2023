CC = gcc
CLFAGS = -pthread -Wall -g
TARGET = main.o
SRC = main.c
LIB = lib/lib.c lib/threads.c lib/proc.c lib/queue.c

all:
	$(CC) $(CLFAGS) $(SRC) $(LIB) -o main.o

fifo:
	./main.o -alg FIFO -input docs/ex_input.txt

sjf:
	./main.o -alg SJF -input docs/ex_input.txt

pr:
	./main.o -alg PR -input docs/ex_input.txt

rr:
	./main.o -alg RR -quantum 30 -input docs/ex_input.txt

clean:
	rm -f $(TARGET) *.o
