#include "graph.h"

int EmptyQueue(Queue *Q){
	if(Q->front == NULL && Q->rear == NULL) return 1;  //empty
	else return 0;
}

void EnQueue(Queue *Q, int e){     //insert e into end of Q
	List *p = (List*)malloc(sizeof(List));
	if(!p)		exit(-1);
	p->index = e;
	p->next = NULL;
	if(Q->front==NULL){     //inserting the first num
        Q->front=p;
        Q->rear = p;
	}
	else{   //inserting other nums
        Q->rear->next = p;
        Q->rear = p;
	}
}

void DeQueue(Queue *Q, int *e){     //delete the first num from Q
	List *p = Q->front;
	if(Q->front==NULL){
        printf("queue empty\n");    return ;
	}
	*e = Q->front->index;
	Q->front = Q->front->next;
	if(Q->front == NULL)    //deleting the last num
		Q->rear = NULL;
	free(p);
}

void breadthFirstSearch(int s, Graph* graph, short *visited, int *save){
    int u;
    Queue Q;        //Queue for BFS
    Q.front=Q.rear=NULL;
    EnQueue(&Q, s);             //s itself is reachable
    while(!EmptyQueue(&Q)){     //Q isn't empty means there are still nodes not visited
        u = Q.front->index;
        visited[u] = 1;         //prevent self-pointing/cycle
        List *p = graph->table[u].outlist;
        while(p != NULL) {      //all targets of current node is reachable,
            if(!visited[p->index]){     //but only save those not yet visited,
                EnQueue(&Q, p->index);
                visited[p->index] = 1;  //then will be visited later.
            }
            p = p->next;        //save num in Q first, deal with them later
        }
        DeQueue(&Q, &u);        //throw away current head num since all stuffs have been done with it
        save[0]++;              //save head num in reachable list
        save[save[0]]=u;
    }
}

void depthFirstSearch(int s, Graph* graph, short *visited, int *save){
    save[0]++;
    visited[s] = 1;     //prevent self-pointing/cycle
    save[save[0]]=s;    //save current num in reachable list
    List *p = graph->table[s].outlist;
    while(p != NULL){
        if(!visited[p->index]){
            depthFirstSearch(p->index, graph, visited, save);
        }
        p = p->next;    //deal with them directly
    }
}

int main(int argc,char *argv[]){
    Graph mygraph;
    read_graph(&mygraph,argv[1]);
    int *save = (int*)calloc(mygraph.MaxSize, sizeof(int));     //for saving lists of reachable node indices
    if(!save)
        exit(-1);
    short i, out_max, out_min = mygraph.MaxSize, j, *visited = (short *)calloc(mygraph.MaxSize, sizeof(short));    //for marking node visiting state
    if(!visited)
        exit(-1);
    for(i=1; i<mygraph.MaxSize; i++){
        if(mygraph.table[i].outdegree<out_min&&mygraph.table[i].outdegree!=0)   //find minimum out degree
            out_min = mygraph.table[i].outdegree;
    }
    for(i=1; i<mygraph.MaxSize; i++)
        if(mygraph.table[i].outdegree==out_min)    //execute on every node that has an out degree equal to the minimum
            break;
    printf("The node with the smallest (non-zero) out-degree is %hd with out-degree %hd\n", i, out_min);

    #ifdef BFS
    breadthFirstSearch(i, &mygraph, visited, save);   //reachable node are saved in 'save'
    #endif
    #ifdef DFS
    depthFirstSearch(i, &mygraph, visited, save);   //reachable node are saved in 'save'
    #endif
    //printf("\nList=");for(j=1;j<=save[0];j++)    printf("%d ", save[j]);  putchar('\n');

    out_max = 0;
    for(j=1;j<=save[0];j++){
        if(mygraph.table[save[j]].outdegree>out_max)    //find maximum out degree within list of reachable nodes
            out_max = mygraph.table[save[j]].outdegree;
    }
    for(j=1;j<=save[0];j++){
        if(mygraph.table[save[j]].outdegree==out_max)   //find nodes that has an out degree equal to the maximum
            break;
    }
    printf("There are %d nodes reachable from the node with the\nsmallest (non-zero) out-degree and the node with the\nlargest out-degree is %s with out-degree %hd ", save[0], mygraph.table[save[j]].name, out_max);

    free(save);
    free(visited);
    destroyGraph(&mygraph);
    return 0;
}
