#include "MinPQ.h"
#include "Graph.h"
#include "Helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    if (nmatched != 3)
    {
        fprintf(stderr, "Invalid file format");
        exit(-1);
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
 * @return struct MinPQ* 
 */
struct MinPQ* InitializePriorityQueue(struct Graph* graph)
{
    struct MinPQ* queue = CreateMinPQ(graph -> numberOfVertices); // ! O(1)
    int heapIndex;
    for (int index = 0 ; index < graph -> numberOfVertices ; index++) // ! O(VlgV)
    {
        heapIndex = PQInsert(queue, graph, index + 1); // ! O(lgV)
        if (heapIndex != -1)
        {
            graph -> adjacencyList[index].heapIndex = heapIndex;
            //PrintMinPQ(queue);
        }
        else 
        {
            fprintf(stderr, "Queue is full!");
            exit(-1);
        }
    }
    
    return queue;
}

void FindMinimumWeaknessPath(struct Graph* graph, struct MinPQ* queue)
{
    int strongestVertex, neighbourId, neighbourHeapIndex, neighbourGraphIndex;
    struct ListNode* current;
    double linkWeakness, vertexWeakness, neighbourWeakness, totalWeakness;
    bool isVisited;
    while (queue -> numberOfElements > 0)
    {
        strongestVertex = PQExtractMin(queue, graph);
        //PrintMinPQ(queue);
        //PrintGraph(graph);
        //printf("VISITING VERTEX %d\n", strongestVertex);
        graph -> adjacencyList[strongestVertex - 1].heapIndex = -1;
        vertexWeakness = graph -> adjacencyList[strongestVertex - 1].weight;
        current = graph -> adjacencyList[strongestVertex - 1].list;
        while (current != NULL)
        {
            neighbourId = current -> vertexId;
            neighbourGraphIndex = neighbourId - 1;
            isVisited = graph -> adjacencyList[neighbourGraphIndex].heapIndex == -1;
            if (!isVisited)
            {
                //printf("Vertex %d is not visited.\n", neighbourId);
                linkWeakness = current -> linkWeight;
                neighbourWeakness = graph -> adjacencyList[neighbourGraphIndex].weight;
                totalWeakness = vertexWeakness + linkWeakness;
                //printf("Total Weakness: %lf, Neighbour Weakness: %lf.\n", totalWeakness, neighbourWeakness);
                if (totalWeakness < neighbourWeakness)
                {
                    neighbourHeapIndex = graph -> adjacencyList[neighbourGraphIndex].heapIndex;
                    //printf("NEIGHBOUR GRAPH INDEX: %d, NEIGHBOUR HEAP INDEX: %d\n", neighbourGraphIndex, neighbourHeapIndex);
                    int returnValue = PQDecreaseKey(queue, graph, neighbourHeapIndex, totalWeakness);
                    if (returnValue == 0)
                        graph -> adjacencyList[neighbourGraphIndex].previousVertexId = strongestVertex;
                    else if (returnValue == -1)
                    {
                        fprintf(stderr, "Index Out Of Bounds: Heap index is %d where heap size is %d and heap capacity is %d", neighbourHeapIndex, queue -> numberOfElements, queue -> capacity); 
                        exit(-1);
                    }
                    else if (returnValue == -2)
                    {
                        fprintf(stderr, "(Source Vertex: %d): Current key %lf of neighbour vertex %d is already smaller than %lf!", strongestVertex, neighbourWeakness, neighbourId, totalWeakness); 
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
    }
    int srcVertex;
    for (int index = 1 ; index < graph -> numberOfVertices ; index++)
    {
        srcVertex = index + 1;
        struct PathNode* shortestPath = (struct PathNode*) malloc(sizeof(struct PathNode));
        shortestPath -> vertexId = srcVertex;
        shortestPath -> next = NULL;

        int prevVertex = graph -> adjacencyList[shortestPath -> vertexId - 1].previousVertexId;
        while (prevVertex != -1)
        {
            struct PathNode* prevNode = (struct PathNode*) malloc(sizeof(struct PathNode));
            prevNode -> vertexId = prevVertex;
            prevNode -> next = shortestPath;
            shortestPath = prevNode;
            prevVertex = graph -> adjacencyList[shortestPath -> vertexId - 1].previousVertexId;
        }

        printf("Shortest Path From Vertex 1 to Vertex %d:\n", srcVertex);
        struct PathNode* next = shortestPath;
        while (shortestPath -> next != NULL)
        {
            printf("%d -> ", shortestPath -> vertexId);
            next = shortestPath -> next;
            shortestPath -> next = NULL;
            free(shortestPath);
            shortestPath = next;
        }
        printf("%d\n", shortestPath -> vertexId);
        free(shortestPath);
        shortestPath = NULL;
    }
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
    const char* fileName = "test.mtx";//argv[1];
    struct Graph* graph = FileToGraph(fileName); // ! O(E)
    //PrintGraph(graph);
    struct MinPQ* queue = InitializePriorityQueue(graph); // ! O(VlgV)
    //PrintMinPQ(queue);
    //PrintGraph(graph);
    
    FindMinimumWeaknessPath(graph, queue);
    //PrintMinPQ(queue);
    //PrintGraph(graph);

    DestroyMinPQ(queue); // ! O(1)
    queue = NULL;
    DestroyGraph(graph); // ! O(E)
    graph = NULL;
    printf("Hello File %s\n", fileName);
    return 0;
}