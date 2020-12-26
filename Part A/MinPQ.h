#ifndef __MINPQ_H__
#define __MINPQ_H__
#include <stdio.h>
#include <stdlib.h>

struct MinPQ {
    int numberOfElements;
    int capacity;
    int* minHeap;
};

struct MinPQ* CreateMinPQ(const int capacity);

//void Insert(struct MinPQ* queue, struct Vertex* vertex);

//void DecreaseKey(struct MinPQ* queue, int index, double newKey);

//struct Vertex* ExtractMin(struct MinPQ* queue);

#endif