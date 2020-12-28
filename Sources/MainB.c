#include "MaxPQ.h"
#include "GraphB.h"
#include "HelperB.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

struct PathNode
{
    int vertexId;
    struct PathNode* next;
};
/**
 * @brief Create Graph form File
 * ! Complexity: O(E)
 * @param fileName 
 * @return struct Graph* 
 */
struct Graph* FileToGraph(const char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file");
        exit(-1);
    }
    int nmatched, vertex1, vertex2, nvertices, nedges;
    double linkWeight;


    nmatched = fscanf(file, "%d %d %d", &nvertices, &nvertices, &nedges);
    while (nmatched != 3)
    {
        nmatched = fscanf(file, "%d %d %d", &nvertices, &nvertices, &nedges);
    }
    struct Graph* graph = CreateGraph(nvertices);
    nmatched = fscanf(file, "%d %d %lf", &vertex1, &vertex2, &linkWeight);
    while (nmatched == 3)
    {
        //printf("Adding Edge %d -> %d\n", vertex1, vertex2);
        AddEdgeToGraph(graph, vertex1, vertex2, linkWeight);
        nmatched = fscanf(file, "%d %d %lf", &vertex1, &vertex2, &linkWeight);
    }
    fclose(file);
    return graph;
}

/**
 * @brief Create the initial priority queue from the given graph
 * ! Complexity: O(VlgV)
 * @param graph 
 * @return struct MaxPQ* 
 */
struct MaxPQ* InitializePriorityQueue(struct Graph* graph)
{
    struct MaxPQ* queue = CreateMaxPQ(graph -> numberOfVertices); // ! O(1)
    int heapIndex;
    for (int index = 0 ; index < graph -> numberOfVertices ; index++) // ! O(VlgV)
    {
        heapIndex = PQInsert(queue, graph, index + 1); // ! O(lgV)
        if (heapIndex != -1)
        {
            graph -> adjacencyList[index].heapIndex = heapIndex;
            //PrintMaxPQ(queue);
        }
        else 
        {
            fprintf(stderr, "Queue is full!");
            exit(-1);
        }
    }
    
    return queue;
}

void RunDijkstra(struct Graph* graph, struct MaxPQ* queue)
{
    int mostReliableVertex, neighbourId, neighbourHeapIndex, neighbourGraphIndex;
    struct ListNode* current;
    double linkReliability, vertexReliability, neighbourReliability, totalReliability;
    bool isVisited;
    while (queue -> numberOfElements > 0)
    {
        mostReliableVertex = PQExtractMax(queue, graph);
        if (SINGLE_STEPPING)
        {
            printf("-------------BEGIN--------------\n");
            PrintMaxPQ(queue);
            PrintGraph(graph);
            printf("VISITING VERTEX %d\n", mostReliableVertex);
        }
        vertexReliability = graph -> adjacencyList[mostReliableVertex - 1].weight;
        current = graph -> adjacencyList[mostReliableVertex - 1].list;
        while (current != NULL)
        {
            neighbourId = current -> vertexId;
            neighbourGraphIndex = neighbourId - 1;
            isVisited = graph -> adjacencyList[neighbourGraphIndex].heapIndex == -1;
            if (!isVisited)
            {
                if (SINGLE_STEPPING)
                    printf("Vertex %d is not visited.\n", neighbourId);
                linkReliability = current -> linkWeight;
                neighbourReliability = graph -> adjacencyList[neighbourGraphIndex].weight;
                totalReliability = vertexReliability * linkReliability;
                if (SINGLE_STEPPING)
                    printf("Reliability: %lf, Neighbour Reliability: %lf.\n", totalReliability, neighbourReliability);
                if (totalReliability > neighbourReliability)
                {
                    neighbourHeapIndex = graph -> adjacencyList[neighbourGraphIndex].heapIndex;
                    //printf("NEIGHBOUR GRAPH INDEX: %d, NEIGHBOUR HEAP INDEX: %d\n", neighbourGraphIndex, neighbourHeapIndex);
                    int returnValue = PQIncreaseKey(queue, graph, neighbourHeapIndex, totalReliability);
                    if (returnValue == 0)
                        graph -> adjacencyList[neighbourGraphIndex].previousVertexId = mostReliableVertex;
                    else if (returnValue == -1)
                    {
                        fprintf(stderr, "Index Out Of Bounds: Heap index is %d where heap size is %d and heap capacity is %d", neighbourHeapIndex, queue -> numberOfElements, queue -> capacity); 
                        exit(-1);
                    }
                    else if (returnValue == -2)
                    {
                        fprintf(stderr, "(Source Vertex: %d): Current key %lf of neighbour vertex %d is already larger than %lf!", mostReliableVertex, neighbourReliability, neighbourId, totalReliability); 
                        exit(-1);
                    }    
                    else
                    {
                        fprintf(stderr, "Return value is %d for an unknown reason.", returnValue);  
                        exit(-1);
                    }   
                }
            }
            current = current -> next;
        }
        if (SINGLE_STEPPING)
        {
            PrintMaxPQ(queue);
            PrintGraph(graph);
            printf("VISITING VERTEX %d\n-------------END--------------\n", mostReliableVertex);
        }
    }
    PrintGraph(graph);
}

void FindMaximumReliabilityPaths(struct Graph* graph, struct MaxPQ* queue)
{
    int srcVertex;
    for (int index = 1 ; index < graph -> numberOfVertices ; index++)
    {
        srcVertex = index + 1;
        struct PathNode* longestPath = (struct PathNode*) malloc(sizeof(struct PathNode));
        longestPath -> vertexId = srcVertex;
        longestPath -> next = NULL;

        int prevVertex = graph -> adjacencyList[longestPath -> vertexId - 1].previousVertexId;
        while (prevVertex != -1)
        {
            struct PathNode* prevNode = (struct PathNode*) malloc(sizeof(struct PathNode));
            prevNode -> vertexId = prevVertex;
            prevNode -> next = longestPath;
            longestPath = prevNode;
            prevVertex = graph -> adjacencyList[longestPath -> vertexId - 1].previousVertexId;
        }

        printf("Longest Path From Vertex 1 to Vertex %d:\n", srcVertex);
        struct PathNode* next = longestPath;
        while (longestPath -> next != NULL)
        {
            printf("%d -> ", longestPath -> vertexId);
            next = longestPath -> next;
            longestPath -> next = NULL;
            free(longestPath);
            longestPath = next;
        }
        printf("%d\n", longestPath -> vertexId);
        free(longestPath);
        longestPath = NULL;
    }
}

void CreateFillFile(struct Graph* graph, char* fileName)
{
    FILE* file = fopen(fileName, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Cannot open file %s\n", fileName);
        exit(-1);
    }
    double weight;
    for (int index = 0 ; index < graph -> numberOfVertices ; index++)
    {
        weight = graph -> adjacencyList[index].weight;
        if (weight == 0.0)
        {
            weight = -1;
            fprintf(file, "%d\n", -1);
        }
        else
        {
            fprintf(file, "%0.8lf\n", weight);
        }
        
            
        
        // ! CANNOT CHECK DEAD-ENDS !!!
    }
    fclose(file);
}


/**
 * @brief Main Method
 * ! Complexity: O(E + VlgV) currently
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[])
{
    // Read .mtx file and create the graph
    if (argc != 2)
    {
        fprintf(stderr, "Invalid number of arguments");
        exit(-1);
    }
    const char* fileName = argv[1];
    struct Graph* graph = FileToGraph(fileName); // ! O(E)
    struct MaxPQ* queue = InitializePriorityQueue(graph); // ! O(VlgV)
    
    RunDijkstra(graph, queue);
    FindMaximumReliabilityPaths(graph, queue);
    CreateFillFile(graph, "b.txt");

    DestroyMaxPQ(queue); // ! O(1)
    queue = NULL;
    DestroyGraph(graph); // ! O(E)
    graph = NULL;
    printf("Hello File %s\n", fileName);
    return 0;
}