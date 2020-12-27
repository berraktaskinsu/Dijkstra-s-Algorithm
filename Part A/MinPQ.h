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

int PQInsert(struct MinPQ* queue, struct Graph* graph, const int vertexId);

int PQDecreaseKey(struct MinPQ* queue, struct Graph* graph, int index, const double key);

void PrintMinPQ(struct MinPQ* queue);

void DestroyMinPQ(struct MinPQ* queue);



void MinHeapify(struct MinPQ* queue, struct Graph* graph, const int index);

double GetKeyOfVertex(struct Graph* graph, const int vertexId);

int GetVertexOfHeapIndex(struct MinPQ* queue, const int heapIndex);

double GetKeyOfHeapIndex(struct MinPQ* queue, struct Graph* graph, const int heapIndex);

int Parent(const int index);

int LeftChild(const int index);

int RightChild(const int index);

#endif