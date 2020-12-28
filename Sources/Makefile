CC = gcc
CFLAGS = -g -Wall
LIBS = -lm

default: clean A B

A: MainA.o GraphA.o MinPQ.o
	$(CC) $(CFLAGS) -o A MainA.o GraphA.o MinPQ.o $(LIBS)

MainA.o: MainA.c GraphA.h MinPQ.h HelperA.h
	$(CC) $(CFLAGS) -c MainA.c

GraphA.o: GraphA.c GraphA.h HelperA.h
	$(CC) $(CFLAGS) -c GraphA.c

MinPQ.o: MinPQ.c MinPQ.h HelperA.h
	$(CC) $(CFLAGS) -c MinPQ.c


B: MainB.o GraphB.o MaxPQ.o
	$(CC) $(CFLAGS) -o B MainB.o GraphB.o MaxPQ.o $(LIBS)

MainB.o: MainB.c GraphB.h MaxPQ.h HelperB.h
	$(CC) $(CFLAGS) -c MainB.c

GraphB.o: GraphB.c GraphB.h HelperB.h
	$(CC) $(CFLAGS) -c GraphB.c

MaxPQ.o: MaxPQ.c MaxPQ.h HelperB.h
	$(CC) $(CFLAGS) -c MaxPQ.c

clean: 
	$(RM) A B *.o *~