#include "MinPQ.h"
#include "Graph.h"
#include "Helper.h"
#include <stdio.h>
#include <stdlib.h>


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
    do
    {
        nmatched = fscanf(file, "%d %d %lf", &vertex1, &vertex2, &linkWeight);
        AddEdgeToGraph(graph, vertex1, vertex2, linkWeight);
    } while (nmatched == 3);
    fclose(file);
    return graph;
}

struct MinPQ* InitializePriorityQueue(struct Graph* graph)
{
    return CreateMinPQ(graph -> numberOfVertices);
}

/**
 * @brief Main Method
 * ! Complexity: O(E) currently
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
    const char* fileName = "medium.mtx";//argv[1];
    struct Graph* graph = FileToGraph(fileName);
    //struct MinPQ* queue = InitializePriorityQueue(graph);

    DestroyGraph(graph);
    graph = NULL;
    printf("Hello File %s", fileName);
    return 0;
}