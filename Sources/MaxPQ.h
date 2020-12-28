#ifndef __MAXPQ_H__
#define __MAXPQ_H__
#include "GraphB.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


struct MaxPQ {
    int numberOfElements;
    int capacity;
    int* maxHeap;
};

struct MaxPQ* CreateMaxPQ(const int capacity);

int PQExtractMax(struct MaxPQ* queue, struct Graph* graph);

int PQInsert(struct MaxPQ* queue, struct Graph* graph, const int vertexId);

int PQIncreaseKey(struct MaxPQ* queue, struct Graph* graph, int index, const double key);

void PrintMaxPQ(struct MaxPQ* queue);

void DestroyMaxPQ(struct MaxPQ* queue);



void MaxHeapify(struct MaxPQ* queue, struct Graph* graph, const int index);

double GetKeyOfVertex(struct Graph* graph, const int vertexId);

int GetVertexOfHeapIndex(struct MaxPQ* queue, const int heapIndex);

double GetKeyOfHeapIndex(struct MaxPQ* queue, struct Graph* graph, const int heapIndex);

int Parent(const int index);

int LeftChild(const int index);

int RightChild(const int index);

#endif