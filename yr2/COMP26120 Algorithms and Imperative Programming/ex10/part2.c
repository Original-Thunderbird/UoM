#include "graph.h"

int EmptyQueue(Queue *Q){
	if(Q->front == NULL && Q->rear == NULL) return 1;  //empty
	else return 0;
}

void InQueue(Queue *Q, int e){     //insert e into end of Q
	List *p = (List*)malloc(sizeof(List));
	if(!p)		exit(-1);
	p->index = e;
	p->next = NULL;
	if(Q->front==NULL){     //inserting the first num
        Q->front=p;     Q->rear = p;
	}
	else{   //inserting other nums
        Q->rear->next = p;      Q->rear = p;
	}
}

void OutQueue(Queue *Q, int *e){     //delete the first num from Q
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

void Heuristic(Graph *graph, int start, short *dist){
    List *p;
    int cur;
    short *visited = (short*)calloc(graph->MaxSize, sizeof(short)), max;  //visited for marking visited nodes
    Queue Q;
    Q.front=Q.rear=NULL;
    for(short i=1; i<=graph->MaxSize; i++){
        dist[i] = INITMAX;      // initial max distance
    }
    dist[start] = 0;
    InQueue(&Q, start); //first node in queue
    while(!EmptyQueue(&Q)){
        max = 0;
        cur = Q.front->index;
        visited[cur] = 1;         //prevent self-pointing/cycle
        p = graph->table[cur].outlist;
        while(p!=NULL){     //get heuristic base for further search
            if(graph->table[p->index].outdegree>max && !visited[p->index])
                max=graph->table[p->index].outdegree;  //record max degree
            p = p->next;
        }
        p = graph->table[cur].outlist;
        while(p != NULL) {      //all targets of current node is reachable,
            if(!visited[p->index] && dist[cur]+1 < dist[p->index]){
                dist[p->index] = dist[cur] + 1;       //update dist
                if(graph->table[p->index].outdegree==max)   //yet only those with local max out degree will be the source later, called heuristic
                    InQueue(&Q, p->index);
                visited[p->index] = 1;  //then will be visited later.
            }
            p = p->next;        //save num in Q first, deal with them later
        }
        OutQueue(&Q, &cur);        //throw away current head num since all stuffs have been done with it
    }
    free(visited);
}

int main(int argc,char *argv[]){
    Graph mygraph;
    read_graph(&mygraph,argv[1]);
    short *dist = (short*)malloc(mygraph.MaxSize*sizeof(short));  //save dist from each node for statistics
    int reachable = 0, count_reach, par_dist, count_rubreak, rulebreak = 0, loc_long_path, glo_long_path=0;
    double glo_dist = 0;
    if(!dist)
        exit(-1);
    FILE *out = fopen("part2-C.txt","w+");
    clock_t start,end;
    start = clock();  //start timing
    for(short i=1;i<mygraph.MaxSize;i++){
        loc_long_path = 0;
        count_reach = 1;  //source itself is reachable
        par_dist = 0;
        count_rubreak = 0;
        fprintf(out, "for node %hd:\n", i);
				printf("for node %hd:\n", i);
        fprintf(out, "nodes reachable with dist>6:\n");
				printf("nodes reachable with dist>6:\n");
        Heuristic(&mygraph, i, dist);
        reachable += count_reach;
        for(short j=1;j<mygraph.MaxSize;j++){
            if(dist[j]!=INITMAX){
                par_dist += dist[j];     //calculate total distance from single source
                if(dist[j]>loc_long_path){
                    loc_long_path = dist[j];
                }
                count_reach++;
                if(dist[j]>6){
                    fprintf(out,"%hd ", j);
                    count_rubreak++;  // count num of nodes with distance larger than 6
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
        fprintf(out, "\n%d in total\n",mygraph.MaxSize-count_reach);  //num of nodes that are unreachable
				printf("\n%d in total\n",mygraph.MaxSize-count_reach);
        fprintf(out, "average distance: %f\n\n", par_dist *1.0/count_reach);
				printf("average distance: %f\n", par_dist *1.0/count_reach);
        glo_dist += par_dist *1.0/count_reach;  //count global total distance
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
