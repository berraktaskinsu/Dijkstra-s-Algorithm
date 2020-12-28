#include "MinPQ.h"
#include "GraphA.h"
#include "HelperA.h"

/**
 * @brief Create a MaxPQ object
 * ! Complexity: O(1)
 * @param capacity 
 * @return struct MaxPQ* 
 */
struct MaxPQ* CreateMaxPQ(const int capacity)
{
    struct MaxPQ* queue = (struct MaxPQ*) malloc(sizeof(struct MaxPQ));
    queue -> capacity = capacity;
    queue -> numberOfElements = 0;
    queue -> maxHeap = (int*) malloc(capacity * sizeof(int));
    return queue;
}

/**
 * @brief Extract the maximum-key element from a MaxPQ object & restore heap property
 * ! Complexity: O(lgV)
 * @param queue 
 * @param graph 
 * @return int 
 */
int PQExtractMax(struct MaxPQ* queue, struct Graph* graph)
{
    int numberOfElements = queue -> numberOfElements;
    int maxVertexId = -1;
    if (numberOfElements > 0)
    {
        maxVertexId = GetVertexOfHeapIndex(queue, 0);
        queue -> maxHeap[0] = GetVertexOfHeapIndex(queue, numberOfElements - 1);
        queue -> numberOfElements --;
        graph -> adjacencyList[maxVertexId - 1].heapIndex = -1;
        graph -> adjacencyList[queue -> maxHeap[0] - 1].heapIndex = 0;
        MaxHeapify(queue, graph, 0); // ! O(lgV)
    }
    return maxVertexId;   
}

/**
 * @brief Insert an element to a MaxPQ object & restore heap property
 * ! Complexity: O(lgV)
 * @param queue 
 * @param graph 
 * @param vertexId 
 * @return int 
 */
int PQInsert(struct MaxPQ* queue, struct Graph* graph, const int vertexId)
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
            int parentIndex = Parent(index);
            double parentKey = GetKeyOfHeapIndex(queue, graph, parentIndex);
            
            while (index > 0 && key < parentKey)
            {
                //printf("INDEX: %d, PARENT ID: %d, PARENT KEY: %lf\n", index, parentIndex, parentKey);
                queue -> maxHeap[index] = GetVertexOfHeapIndex(queue, parentIndex);
                index = parentIndex;
                parentIndex = Parent(index);
                if (parentIndex >= 0)
                    parentKey = GetKeyOfHeapIndex(queue, graph, parentIndex);
            }
        }
        queue -> maxHeap[index] = vertexId;
    }
    return index;
}

int PQIncreaseKey(struct MaxPQ* queue, struct Graph* graph, int heapIndex, const double key)
{
    if (queue -> numberOfElements <= heapIndex)
        return -1;
    int graphIndex = queue -> maxHeap[heapIndex] - 1;
    double currentKey = graph -> adjacencyList[graphIndex].weight;
    if (SINGLE_STEPPING)
        printf("HEAP INDEX:%d, VERTEX:%d, CURRENT KEY: %lf, KEY: %lf\n", heapIndex, graphIndex + 1, currentKey, key);
    if (currentKey <= key)
        return -2;
    if (heapIndex >= 0)
    {
        if (heapIndex > 0)
        {
            int parentIndex = Parent(heapIndex);
            double parentKey = GetKeyOfHeapIndex(queue, graph, parentIndex);
            //printf("KEY:%lf MYKEY:%lf PARENT KEY:%lf\n", key, currentKey, parentKey);
            while(heapIndex > 0 && key < parentKey)
            {
                queue -> maxHeap[heapIndex] = GetVertexOfHeapIndex(queue, parentIndex);
                graph -> adjacencyList[queue -> maxHeap[heapIndex] - 1].heapIndex = heapIndex;
                heapIndex = parentIndex;
                parentIndex = Parent(heapIndex);
                if (parentIndex >= 0)
                    parentKey = GetKeyOfHeapIndex(queue, graph, parentIndex);
            } 
        }
        
        queue -> maxHeap[heapIndex] = graphIndex + 1;
        graph -> adjacencyList[graphIndex].weight = key;
        graph -> adjacencyList[graphIndex].heapIndex = heapIndex;
    }
    return 0;
}

void PrintMaxPQ(struct MaxPQ* queue)
{
    printf("\nQueue - Number of Vertices: %d\n", queue -> numberOfElements);
    for (int index = 0 ; index < queue -> numberOfElements ; index++)
    {
        printf("Index %d: Vertex %d\n", index, queue -> maxHeap[index]);
    }
}

/**
 * @brief Deallocate and destroy a MaxPQ object
 * ! Complexity: O(1)
 * @param queue 
 */
void DestroyMaxPQ(struct MaxPQ* queue)
{
    free(queue -> maxHeap);
    queue -> maxHeap = NULL;
    free(queue);
}

/**
 * @brief Restore heap property of a MaxPQ object
 * ! Complexity: O(lgV)
 * @param queue 
 * @param graph 
 * @param index 
 */
void MaxHeapify(struct MaxPQ* queue, struct Graph* graph, const int index)
{
    int numberOfElements = queue -> numberOfElements;

    int maxHeapIndex = index;
    double maxKey = GetKeyOfHeapIndex(queue, graph, maxHeapIndex);

    int leftChildIndex = LeftChild(index);
    int rightChildIndex = RightChild(index);
    double leftChildKey, rightChildKey;
    //printf("Left Child Index: %d, Left Child: %d, Right Child Index: %d, RightChild: %d\n", leftChildIndex, GetVertexOfHeapIndex(queue, leftChildIndex), rightChildIndex, GetVertexOfHeapIndex(queue, rightChildIndex));
    //printf("Initial Max Index: %d, Max Value: %lf\n", maxHeapIndex, maxKey);
    if (leftChildIndex < numberOfElements)
    {
        leftChildKey = GetKeyOfHeapIndex(queue, graph, leftChildIndex);
        //printf("Left Child Key: %lf\n", leftChildKey);
        if (leftChildKey < maxKey)
        {
            
            maxHeapIndex = leftChildIndex;
            maxKey = leftChildKey;
        }
    }
    //printf("After Left Max Index: %d, Max Value: %lf\n", maxHeapIndex, maxKey);
    if (rightChildIndex < numberOfElements)
    {
        rightChildKey = GetKeyOfHeapIndex(queue, graph, rightChildIndex);
        //printf("Right Child Key: %lf\n", rightChildKey);
        if (rightChildKey < maxKey)
        {
            maxHeapIndex = rightChildIndex;
            maxKey = rightChildKey;
        }
    }
    //printf("After Right Max Index: %d, Max Value: %lf\n", maxHeapIndex, maxKey);
    if (maxHeapIndex != index)
    {
        // Update Heap Indices
        int temp = GetVertexOfHeapIndex(queue, maxHeapIndex);
        queue -> maxHeap[maxHeapIndex] = GetVertexOfHeapIndex(queue, index);
        queue -> maxHeap[index] = temp;

        graph -> adjacencyList[GetVertexOfHeapIndex(queue, index) - 1].heapIndex = index;
        graph -> adjacencyList[GetVertexOfHeapIndex(queue, maxHeapIndex) - 1].heapIndex = maxHeapIndex;
        //printf("IN MAXHEAPIFY:\n");
        //PrintGraph(graph);
        MaxHeapify(queue, graph, maxHeapIndex);
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
int GetVertexOfHeapIndex(struct MaxPQ* queue, const int heapIndex)
{
    return queue -> maxHeap[heapIndex];
}

/**
 * @brief Get key-value of the vertex at the given heap-index
 * ! Complexity: O(1)
 * @param queue 
 * @param graph 
 * @param heapIndex 
 * @return int 
 */
double GetKeyOfHeapIndex(struct MaxPQ* queue, struct Graph* graph, const int heapIndex)
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