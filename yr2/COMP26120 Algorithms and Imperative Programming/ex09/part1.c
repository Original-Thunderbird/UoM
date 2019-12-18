#include "graph.h"

int main(int argc,char *argv[]){
    Graph mygraph;
    read_graph(&mygraph,argv[1]);
    short i;
    short out_max = 0, in_max = 0, in_min = mygraph.MaxSize;
    for(i=1; i<mygraph.MaxSize; i++){
        if(mygraph.table[i].outdegree>out_max)     //get maximum out degree
            out_max = mygraph.table[i].outdegree;
        if(mygraph.table[i].indegree>in_max)       //get maximum in degree
            in_max = mygraph.table[i].indegree;
        if(mygraph.table[i].indegree<in_min &&mygraph.table[i].indegree !=0)       //get maximum in degree
            in_min = mygraph.table[i].indegree;
    }
    printf("Max out-degree is %hd at node", out_max);
    for(i=1; i<mygraph.MaxSize; i++){
        if(mygraph.table[i].outdegree==out_max){     //get nodes with out degree equals to the maximum
            printf(" %s", mygraph.table[i].name);
            break;
        }
    }
    printf("\nMax in-degree is %hd at node", in_max);
    for(i=1; i<mygraph.MaxSize; i++){
        if(mygraph.table[i].indegree==in_max){       //get nodes with in degree equals to the maximum
            printf(" %s",mygraph.table[i].name);
            break;
        }
    }
    printf("\nMin in-degree is %hd at node", in_min);
    for(i=1; i<mygraph.MaxSize; i++){
        if(mygraph.table[i].indegree==in_min){       //get nodes with in degree equals to the minimum
            printf(" %s",mygraph.table[i].name);
            break;
        }
    }
    destroyGraph(&mygraph);
    return(0);
}
