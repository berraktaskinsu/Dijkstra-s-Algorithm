#ifndef __MINPQ_H__
#define __MINPQ_H__
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


struct MinPQ {
    int numberOfElements;
    int capacity;
    int* minHeap;
};

struct MinPQ* CreateMinPQ(const int capacity);

int PQExtractMin(struct MinPQ* queue, struct Graph* graph);

int PQInsert(struct MinPQ* queue, struct Graph* graph, int vertexId);

bool PQDecreaseKey(struct MinPQ* queue, int index, int key);

void DestroyMinPQ(struct MinPQ* queue);



void MinHeapify(struct MinPQ* queue, struct Graph* graph, int index);

int GetKeyOfVertex(struct Graph* graph, int vertexId);

int GetVertexOfHeapIndex(struct MinPQ* queue, int heapIndex);

int GetKeyOfHeapIndex(struct MinPQ* queue, struct Graph* graph, int heapIndex);

int Parent(int index);

int LeftChild(int index);

int RightChild(int index);

#endif