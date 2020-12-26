#include "MinPQ.h"
#include "Helper.h"

/**
 * @brief Create a MinPQ object
 * 
 * @param capacity 
 * @return struct MinPQ* 
 */
struct MinPQ* CreateMinPQ(const int capacity)
{
    struct MinPQ* queue = (struct MinPQ*) malloc(sizeof(struct MinPQ));
    queue -> capacity = capacity;
    queue -> minHeap = (int*) malloc(capacity * sizeof(int));
    return queue;
}

/*void Insert(struct MinPQ* queue, struct Vertex* vertex)
{

}

void DecreaseKey(struct MinPQ* queue, int index, double newKey)
{

}

struct Vertex* ExtractMin(struct MinPQ* queue)
{

}*/