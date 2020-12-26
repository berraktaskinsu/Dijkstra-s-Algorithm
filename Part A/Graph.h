#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

struct ListNode {
    int vertexId;
    double linkWeight;
    struct ListNode* next;
};

struct GraphNode {
    double weight;
    int heapIndex;
    int previousVertexId;
    struct ListNode* list;
};

struct Graph {
    int numberOfVertices;
    struct GraphNode* adjacencyList;
};

// Public Methods:
struct Graph* CreateGraph(const int numberOfVertices);

void AddEdgeToGraph(struct Graph* graph, const int srcId, const int dstId, const double weakness);

void DestroyGraph(struct Graph* graph);
// Private Methods:



#endif