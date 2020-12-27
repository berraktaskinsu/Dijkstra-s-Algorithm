#include "MinPQ.h"
#include "Graph.h"
#include "Helper.h"

/**
 * @brief Create a MinPQ object
 * ! Complexity: O(1)
 * @param capacity 
 * @return struct MinPQ* 
 */
struct MinPQ* CreateMinPQ(const int capacity)
{
    struct MinPQ* queue = (struct MinPQ*) malloc(sizeof(struct MinPQ));
    queue -> capacity = capacity;
    queue -> numberOfElements = 0;
    queue -> minHeap = (int*) malloc(capacity * sizeof(int));
    return queue;
}

/**
 * @brief Extract the minimum-key element from a MinPQ object & restore heap property
 * ! Complexity: O(lgV)
 * @param queue 
 * @param graph 
 * @return int 
 */
int PQExtractMin(struct MinPQ* queue, struct Graph* graph)
{
    int numberOfElements = queue -> numberOfElements;
    int minVertexId = -1;
    if (numberOfElements > 0)
    {
        minVertexId = GetVertexOfHeapIndex(queue, 0);
        queue -> minHeap[0] = GetVertexOfHeapIndex(queue, numberOfElements - 1);
        queue -> numberOfElements --;
        graph -> adjacencyList[minVertexId - 1].heapIndex = -1;
        graph -> adjacencyList[queue -> minHeap[0] - 1].heapIndex = 0;
        MinHeapify(queue, graph, 0); // ! O(lgV)
    }
    return minVertexId;   
}

/**
 * @brief Insert an element to a MinPQ object & restore heap property
 * ! Complexity: O(lgV)
 * @param queue 
 * @param graph 
 * @param vertexId 
 * @return int 
 */
int PQInsert(struct MinPQ* queue, struct Graph* graph, const int vertexId)
{
    int index = -1;
    if (queue -> numberOfElements < queue -> capacity)
    {
        queue -> numberOfElements ++;
        /*if (SINGLE_STEPPING)
            printf("VERTEX ID: %d\n", vertexId);*/
        const double key = GetKeyOfVertex(graph, vertexId);
        /*if (SINGLE_STEPPING)
            printf("KEY: %lf\n", key);*/
        index = queue -> numberOfElements - 1;

        if (index > 0)
        {
            int parentIndex = Parent(index);;
            double parentKey = GetKeyOfHeapIndex(queue, graph, parentIndex);
            
            while (index > 0 && key < parentKey)
            {
                printf("INDEX: %d, PARENT ID: %d, PARENT KEY: %lf\n", index, parentIndex, parentKey);
                queue -> minHeap[index] = GetVertexOfHeapIndex(queue, parentIndex);
                index = parentIndex;
                parentIndex = Parent(index);
                parentKey = GetKeyOfHeapIndex(queue, graph, parentIndex);
            }
        }
        queue -> minHeap[index] = vertexId;
    }
    return index;
}

int PQDecreaseKey(struct MinPQ* queue, struct Graph* graph, int heapIndex, const double key)
{
    if (queue -> numberOfElements <= heapIndex)
        return -1;
    int graphIndex = queue -> minHeap[heapIndex] - 1;
    double currentKey = graph -> adjacencyList[graphIndex].weight;
    printf("CURRENT KEY: %lf, KEY: %lf\n", currentKey, key);
    if (currentKey <= key)
        return -2;
    if (heapIndex > 0)
    {
        int parentIndex;
        double parentKey;
        do
        {
            parentIndex = Parent(heapIndex);
            parentKey = GetKeyOfHeapIndex(queue, graph, parentIndex);
            queue -> minHeap[heapIndex] = GetVertexOfHeapIndex(queue, parentIndex);
            heapIndex = parentIndex;
        } while(heapIndex > 0 && key > parentKey);
    }
    queue -> minHeap[heapIndex] = graphIndex + 1;
    graph -> adjacencyList[graphIndex].weight = key;
    return 0;
}

void PrintMinPQ(struct MinPQ* queue)
{
    printf("\nQueue - Number of Vertices: %d\n", queue -> numberOfElements);
    for (int index = 0 ; index < queue -> numberOfElements ; index++)
    {
        printf("Index %d: Vertex %d\n", index, queue -> minHeap[index]);
    }
}

/**
 * @brief Deallocate and destroy a MinPQ object
 * ! Complexity: O(1)
 * @param queue 
 */
void DestroyMinPQ(struct MinPQ* queue)
{
    free(queue -> minHeap);
    queue -> minHeap = NULL;
    free(queue);
}

/**
 * @brief Restore heap property of a MinPQ object
 * ! Complexity: O(lgV)
 * @param queue 
 * @param graph 
 * @param index 
 */
void MinHeapify(struct MinPQ* queue, struct Graph* graph, const int index)
{
    int numberOfElements = queue -> numberOfElements;

    int minHeapIndex = index;
    int minKey = GetKeyOfHeapIndex(queue, graph, minHeapIndex);

    int leftChildIndex = LeftChild(index);
    int rightChildIndex = RightChild(index);
    int leftChildKey, rightChildKey;
    
    if (leftChildIndex < numberOfElements)
    {
        leftChildKey = GetKeyOfHeapIndex(queue, graph, leftChildIndex);
        if (leftChildKey < minKey)
        {
            minHeapIndex = leftChildIndex;
            minKey = leftChildKey;
        }
    }

    if (rightChildIndex < numberOfElements)
    {
        rightChildKey = GetKeyOfHeapIndex(queue, graph, rightChildIndex);
        if (rightChildKey < minKey)
        {
            minHeapIndex = rightChildIndex;
            minKey = rightChildKey;
        }
    }

    if (minHeapIndex != index)
    {
        // Update Heap Indices
        

        int temp = GetVertexOfHeapIndex(queue, minHeapIndex);
        queue -> minHeap[minHeapIndex] = GetVertexOfHeapIndex(queue, index);
        queue -> minHeap[index] = temp;

        graph -> adjacencyList[GetVertexOfHeapIndex(queue, index) - 1].heapIndex = index;
        graph -> adjacencyList[GetVertexOfHeapIndex(queue, minHeapIndex) - 1].heapIndex = minHeapIndex;
        MinHeapify(queue, graph, minHeapIndex);
    }
}

/**
 * @brief Get key-value of a vertex
 * ! Complexity: O(1)
 * @param graph 
 * @param vertexId 
 * @return int 
 */
double GetKeyOfVertex(struct Graph* graph, const int vertexId)
{
    int graphIndex = vertexId - 1;
    return graph -> adjacencyList[graphIndex].weight;
}

/**
 * @brief Get vertex-value at the given heap-index
 * ! Complexity: O(1)
 * @param queue 
 * @param heapIndex 
 * @return int 
 */
int GetVertexOfHeapIndex(struct MinPQ* queue, const int heapIndex)
{
    return queue -> minHeap[heapIndex];
}

/**
 * @brief Get key-value of the vertex at the given heap-index
 * ! Complexity: O(1)
 * @param queue 
 * @param graph 
 * @param heapIndex 
 * @return int 
 */
double GetKeyOfHeapIndex(struct MinPQ* queue, struct Graph* graph, const int heapIndex)
{
    return GetKeyOfVertex(graph, GetVertexOfHeapIndex(queue, heapIndex));
}

/**
 * @brief Get the index of the parent of a given index in a heap
 * ! Complexity: O(1)
 * @param index 
 * @return int 
 */
int Parent(const int index)
{
    return floor(((double) index - 1) / 2.0);
}

/**
 * @brief Get the index of the left child of a given index in a heap
 * ! Complexity: O(1)
 * @param index 
 * @return int 
 */
int LeftChild(const int index)
{
    return 2 * (index + 1) - 1;
}

/**
 * @brief Get the index of the right child of a given index in a heap
 *! Complexity: O(1)
 * @param index 
 * @return int 
 */
int RightChild(const int index)
{
    return 2 * (index + 1);
}