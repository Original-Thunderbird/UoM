#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
// #define __USE_BSD // make prototype for strdup visible
#include <string.h>
#include<time.h>

#define INITMAX 32767

typedef struct Elem{
    int id;//Դ����id
    int weight;//��������
}Elem;

typedef struct PriorQ{
    int capacity;
    int size;
    Elem *Elements;
}PriorQ;


typedef struct linkedlist { // linked list of ints (for use in Node)
    int index;
    struct linkedlist *next;
} List;

typedef struct { // a Node of a Graph
    char *name;
    List *outlist; // adjacency list
    int outdegree,indegree; // length of outlist
    //double pagerank_score; //not needed for this exercise
} Node;

typedef struct {
    Node* table;
    short MaxSize; //number of nodes in graph
} Graph;

// use to check result of strdup, malloc etc.
extern void check (void *memory, char *message);
extern void destroyGraph(Graph *mygraph);
extern int initialize_graph (Graph *mygraph, int MaxSize);
extern int insert_graph_node (Graph *mygraph, int n, char *name);
extern int insert_graph_link (Graph *mygraph, int source, int target);
extern int read_graph (Graph *mygraph, char *filename);
extern void print_graph (Graph *mygraph);

extern void breadthFirstSearch(int s, Graph* graph, short *visited, int *save);
extern void depthFirstSearch(int s, Graph* graph, short *visited, int *save);

void Dijkstra(Graph *graph, int v0, short *dist);
void Heuristic(Graph *graph, int v0, short *dist);

typedef struct{
	List *front, *rear;
}Queue;     //queue for BFS

extern void EnQueue(Elem neo, PriorQ *H);
extern void EnQueue(Elem neo, PriorQ *H);
extern Elem DeQueue(PriorQ *H);

extern int EmptyQueue(Queue *Q);
extern void InQueue(Queue *Q, int e);
extern void OutQueue(Queue *Q, int *e);
