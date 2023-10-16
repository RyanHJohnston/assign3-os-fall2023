CC = gcc
CLFAGS = -pthread -Wall -g
TARGET = main.o
SRC = main.c
LIB = lib.c

all:
	$(CC) $(CLFAGS) $(SRC) $(LIB) -o main.o

run:
	

clean:
	rm -f $(TARGET) *.o
