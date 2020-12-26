CC = gcc
CFLAGS = -g -Wall
#LIBS = -lm

default: A

A: Main.o Graph.o MinPQ.o
	$(CC) $(CFLAGS) -o A Main.o Graph.o MinPQ.o

Main.o: Main.c Graph.h MinPQ.h Helper.h
	$(CC) $(CFLAGS) -c Main.c

Graph.o: Graph.c Graph.h Helper.h
	$(CC) $(CFLAGS) -c Graph.c

MinPQ.o: MinPQ.c MinPQ.h Helper.h
	$(CC) $(CFLAGS) -c MinPQ.c

clean: 
	$(RM) A *.o *~