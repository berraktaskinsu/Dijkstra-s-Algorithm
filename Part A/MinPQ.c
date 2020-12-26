#include "MinPQ.h"
#include "Graph.h"
#include "Helper.h"

struct MinPQ* CreateMinPQ(const int capacity)
{
    struct MinPQ* queue = (struct MinPQ*) malloc(sizeof(struct MinPQ));
    queue -> capacity = capacity;
    queue -> minHeap = (int*) malloc(capacity * sizeof(int));
    return queue;
}

int PQExtractMin(struct MinPQ* queue, struct Graph* graph)
{
    int numberOfElements = queue -> numberOfElements;
    int minVertexId = -1;
    if (numberOfElements > 0)
    {
        minVertexId = GetVertexOfHeapIndex(queue, 0);
        queue -> minHeap[0] = GetVertexOfHeapIndex(queue, numberOfElements - 1);
        queue -> numberOfElements --;
        MinHeapify(queue, 0, graph);
    }
    return minVertexId;   
}

int PQInsert(struct MinPQ* queue, struct Graph* graph, int vertexId)
{
    int index = -1;
    if (queue -> numberOfElements < queue -> capacity)
    {
        queue -> numberOfElements ++;
        const int key = GetKeyOfVertex(graph, vertexId);

        int index = queue -> numberOfElements - 1;
        int parentIndex = ParentIndex(index);
        int parentKey = GetKeyOfHeapIndex(graph, queue, parentIndex);

        while (index > 0 && key > parentKey)
        {
            queue -> minHeap[index] = GetVertexOfHeapIndex(queue, parentIndex);
            index = parentIndex;
            parentIndex = ParentIndex(index);
            parentKey = GetKeyOfHeapIndex(graph, queue, parentIndex);
        }
        queue -> minHeap[index] = vertexId;
    }
    return index;
}

bool PQDecreaseKey(struct MinPQ* queue, int index, int key)
{

}

void DestroyMinPQ(struct MinPQ* queue)
{
    free(queue -> minHeap);
    queue -> minHeap = NULL;
    free(queue);
}


void MinHeapify(struct MinPQ* queue, struct Graph* graph, int index)
{
    int numberOfElements = queue -> numberOfElements;

    int minHeapIndex = index;
    int minKey = GetKeyOfHeapIndex(graph, queue, minHeapIndex);

    int leftChildIndex = LeftChildIndex(index);
    int rightChildIndex = RightChildIndex(index);
    int leftChildKey, rightChildKey;
    
    if (leftChildIndex < numberOfElements)
    {
        leftChildKey = GetKeyOfHeapIndex(graph, queue, leftChildIndex);
        if (leftChildKey < minKey)
        {
            minHeapIndex = leftChildIndex;
            minKey = leftChildKey;
        }
    }

    if (rightChildIndex < numberOfElements)
    {
        rightChildKey = GetKeyOfHeapIndex(graph, queue, rightChildIndex);
        if (rightChildKey < minKey)
        {
            minHeapIndex = rightChildIndex;
            minKey = rightChildKey;
        }
    }

    if (minHeapIndex != index)
    {
        int temp = GetVertexOfHeapIndex(queue, minHeapIndex);
        queue -> minHeap[minHeapIndex] = GetVertexOfHeapIndex(queue, index);
        queue -> minHeap[index] = temp;
        MinHeapify(queue, minHeapIndex, graph);
    }
}

int GetKeyOfVertex(struct Graph* graph, int vertexId)
{
    int graphIndex = vertexId - 1;
    return graph -> adjacencyList[graphIndex].weight;
}

int GetVertexOfHeapIndex(struct MinPQ* queue, int heapIndex)
{
    return queue -> minHeap[heapIndex];
}

int GetKeyOfHeapIndex(struct MinPQ* queue, struct Graph* graph, int heapIndex)
{
    return GetKeyOfVertex(graph, GetVertexOfHeapIndex(queue, heapIndex));
}

int Parent(int index)
{
    return floor(((double) index - 1) / 2.0);
}

int LeftChild(int index)
{
    return 2 * (index + 1) - 1;
}

int RightChild(int index)
{
    return 2 * (index + 1);
}