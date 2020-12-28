#include "GraphA.h"
#include "HelperA.h"

// Public Methods:
/**
 * @brief Create a Graph object
 * ! Complexity: O(V)
 * @param numberOfVertices 
 * @return struct Graph* 
 */
struct Graph* CreateGraph(const int numberOfVertices)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph -> numberOfVertices = numberOfVertices;
    graph -> adjacencyList = (struct GraphNode*) malloc(numberOfVertices * sizeof(struct GraphNode));
    for (int index = 0 ; index < numberOfVertices ; index++)
    {
        if (index == 0)
            graph -> adjacencyList[index].weight = IS_MIN ? (double) 0.0 : 1.0;
        else
            graph -> adjacencyList[index].weight = IS_MIN ? (double) INT_MAX : 0.0;
        
        graph -> adjacencyList[index].heapIndex = -1;
        graph -> adjacencyList[index].previousVertexId = -1;
        graph -> adjacencyList[index].list = NULL;
    }

    return graph;
}

/**
 * @brief Add an Edge to a Graph Object
 * ! Complexity: O(1)
 * @param graph 
 * @param srcId 
 * @param dstId 
 * @param linkWeight 
 */
void AddEdgeToGraph(struct Graph* graph, const int srcId, const int dstId, const double linkWeight)
{
    struct ListNode* listNode = (struct ListNode*) malloc(sizeof(struct ListNode));
    listNode -> vertexId = dstId;
    listNode -> linkWeight = linkWeight;
    listNode -> next = graph -> adjacencyList[srcId - 1].list;
    graph -> adjacencyList[srcId - 1].list = listNode;
    /*if (SINGLE_STEPPING)
        printf("Src: %d Dst: %d Link Weight: %lf\n", srcId, dstId, linkWeight);*/
}

void PrintGraph(struct Graph* graph)
{
    printf("\nGraph - Number of Vertices: %d\n", graph -> numberOfVertices);
    for (int index = 0 ; index < graph -> numberOfVertices ; index++)
    {
        printf("\nVertex %d, Weakness: %lf, Prev: %d, Heap Index: %d\n", index + 1, graph -> adjacencyList[index].weight, graph -> adjacencyList[index].previousVertexId, graph -> adjacencyList[index].heapIndex);
        struct ListNode* current = graph -> adjacencyList[index].list;
        while (current != NULL)
        {
            printf("%d -> %d Link Weakness: %lf\n", index + 1, current -> vertexId, current -> linkWeight);
            current = current -> next;
        }
    }
}


/**
 * @brief Deallocate and Destroy a Graph Object
 * ! Complexity: O(E)
 * @param graph 
 */
void DestroyGraph(struct Graph* graph)
{
    int numberOfVertices = graph -> numberOfVertices;
    for (int index = 0 ; index < numberOfVertices ; index++)
    {
        // For each vertex (struct GraphNode):
        struct ListNode* nextListNode = graph -> adjacencyList[index].list;
        while (graph -> adjacencyList[index].list != NULL)
        {
            nextListNode = graph -> adjacencyList[index].list -> next;
            graph -> adjacencyList[index].list -> next = NULL;
            free(graph -> adjacencyList[index].list);
            graph -> adjacencyList[index].list = nextListNode;
        }
    }
    free(graph -> adjacencyList);
    graph -> adjacencyList = NULL;
    free(graph);
}


// Private Methods: