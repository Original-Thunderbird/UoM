#include "graph.h"

PriorQ* Initialize(int MaxElements){
    PriorQ *H = (PriorQ*)malloc(sizeof(PriorQ));
    if (H == NULL){
        printf("Out of space\n"); 	//Allocate the array plus one extra for sentinel
        exit(-1);
    }
    H->Elements = (Elem*)malloc((MaxElements)*sizeof(Elem));
    if (H->Elements == NULL){
        printf("Out of space\n");
        exit(-1);
    }
    H->capacity = MaxElements;  //static allocation
    H->size = 0;
    H->Elements[0].weight = -32768;
    return H;
} //H->Elements[0] is a sentinel

void EnQueue(Elem neo, PriorQ *H){
    int i;
    if (H->size>H->capacity){
        printf( "Priority queue is full\n" );
        return ;
    }
    for (i=++H->size; H->Elements[i/2].weight>neo.weight; i/=2) //The new element is percolated up the heap,size of heap++
        H->Elements[i] = H->Elements[i/2];           //until the correct location is found
    H->Elements[i] = neo;
}

Elem DeQueue(PriorQ *H){
    int i, child;
    Elem MinElement, LastElement;
    if (H->size == 0){
        printf("Priority queue is empty\n");
        return H->Elements[0];
    }
    MinElement=H->Elements[1];
    LastElement=H->Elements[H->size--];     //size of heap--
    for (i=1; i*2 <= H->size; i=child){		//Find smaller child
        child = i*2;
        if (child!=H->size && H->Elements[child+1].weight<H->Elements[child].weight)
            child++; 		//go to right child
        if (LastElement.weight>H->Elements[child].weight)
            H->Elements[i] = H->Elements[child];  //Percolate one level
        else
            break;
        }
    H->Elements[i] = LastElement;
    return MinElement;
}


void Dijkstra(Graph *graph, int start, short *dist){
    Elem buf;
    int vert, cur;
    short *visited = (short*)calloc(graph->MaxSize, sizeof(short));    //visited for marking visited nodes
    PriorQ *Q = Initialize(graph->MaxSize);
    for(short i=1; i<=graph->MaxSize; i++){
        dist[i] = INITMAX;      // initial max distance
    }
    dist[start] = 0;
    buf.id = start;
    buf.weight = dist[start];
    EnQueue(buf,Q);   //first node in queue
    while(Q->size!=0){
        Elem cd = DeQueue(Q);
        cur = cd.id;
        if(visited[cur])            continue;
        visited[cur] = 1;
        List *adj = graph->table[cur].outlist;
        while(adj){  //pick discovery edge for every node
            vert = adj->index;
            if(!visited[vert] && dist[cur]+1 < dist[vert]){
                dist[vert] = dist[cur] + 1;      //update dist
                buf.id = vert;
                buf.weight = dist[vert];
                EnQueue(buf,Q);  //only enqueue those are not in cloud and shorter list dist discovered
            }
            adj = adj->next;
        }
    }
    free(visited);
    free(Q->Elements);
    free(Q);
}

int main(int argc,char *argv[]){
    Graph mygraph;
    read_graph(&mygraph,argv[1]);
    short *dist = (short*)malloc(mygraph.MaxSize*sizeof(short));    //save dist from each node for statistics
    int reachable = 0, count_reach, par_dist, count_rubreak, rulebreak = 0, loc_long_path, glo_long_path=0;
    double glo_dist = 0;
    if(!dist)
        exit(-1);
    FILE *out = fopen("part1-C.txt","w+");
    clock_t start,end;
    start = clock();  //start timing
    for(short i=1;i<mygraph.MaxSize;i++){
        loc_long_path = 0;
        count_reach = 1;    //source itself is reachable
        par_dist = 0;
        count_rubreak = 0;
        fprintf(out, "for node %hd:\n", i);
        printf("for node %hd:\n", i);
        fprintf(out, "nodes reachable with dist>6:\n");
        printf("nodes reachable with dist>6:\n");
        Dijkstra(&mygraph, i, dist);
        reachable += count_reach;
        for(short j=1;j<mygraph.MaxSize;j++){
            if(dist[j]!=INITMAX){
                par_dist += dist[j];       //calculate total distance from single source
                if(dist[j]>loc_long_path){
                    loc_long_path = dist[j];
                }
                count_reach++;
                if(dist[j]>6){
                    fprintf(out,"%hd ", j);
                    count_rubreak++;    // count num of nodes with distance larger than 6
                }
            }
        }
        fprintf(out, "\n%d in total,length of longest shortest path: %d\n",count_rubreak, loc_long_path);
        printf("\n%d in total,length of longest shortest path: %d\n",count_rubreak, loc_long_path);
        fprintf(out, "nodes unreachable:\n");
        printf("nodes unreachable:\n");
        for(short j=1;j<mygraph.MaxSize;j++){
            if(dist[j]==INITMAX)
                fprintf(out,"%hd ", j);
        }
        fprintf(out, "\n%d in total\n",mygraph.MaxSize-count_reach);   //num of nodes that are unreachable
        printf("\n%d in total\n",mygraph.MaxSize-count_reach);
        fprintf(out, "average distance: %f\n\n", par_dist *1.0/count_reach);
        printf("average distance: %f\n\n", par_dist *1.0/count_reach);
        glo_dist += par_dist *1.0/count_reach;    //count global total distance
        if(glo_long_path<loc_long_path)
            glo_long_path = loc_long_path;
        rulebreak+=count_rubreak;
    }
    end = clock();  //end timing
    glo_dist/=mygraph.MaxSize;
    fprintf(out, "time=%f,global average distance=%f, number of distances distances that are larger than 6: %d, length of global longest shortest path: %d\
            ",(double)(end-start)/CLOCKS_PER_SEC, glo_dist, rulebreak, glo_long_path);
    printf("time=%f,global average distance=%f, number of distances distances that are larger than 6: %d, length of global longest shortest path: %d\
            ",(double)(end-start)/CLOCKS_PER_SEC, glo_dist, rulebreak, glo_long_path);
    free(dist);
    fclose(out);
    destroyGraph(&mygraph);
    return 0;
}
