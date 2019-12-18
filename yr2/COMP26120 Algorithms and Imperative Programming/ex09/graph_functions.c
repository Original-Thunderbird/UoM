#include "graph.h"

int initialize_graph (Graph *mygraph, int MaxSize) {
    mygraph->table=(Node*)malloc(MaxSize*sizeof(Node));
    mygraph->MaxSize=MaxSize;
    return 0;
}

int insert_graph_node (Graph *mygraph, int n, char *name) {
    //set up table nodes
    mygraph->table[n].name=(char*)malloc(strlen(name)+1);
    if(!(mygraph->table[n].name))
        exit(-1);
    strcpy(mygraph->table[n].name, name);
    mygraph->table[n].outdegree=mygraph->table[n].indegree=0;
    mygraph->table[n].outlist=NULL;
    return 0;
}

int insert_graph_link (Graph *mygraph, int source, int target) {
    if(mygraph->table[source].outlist==NULL){   //first edge for a node
        mygraph->table[source].outlist=(List*)malloc(sizeof(List));
            if(!(mygraph->table[source].outlist))
                exit(-1);
        mygraph->table[source].outlist->index=target;
        mygraph->table[source].outlist->next=NULL;
    }
    else{   //other edges for a node
        List *p;
        for(p = mygraph->table[source].outlist; p->next; p=p->next);
        p->next=(List*)malloc(sizeof(List));
            if(!(p->next))
                exit(-1);
        p->next->index=target;
        p->next->next=NULL;
    }
    mygraph->table[target].indegree++;      //update relevant in/out degree number for further statistics
    mygraph->table[source].outdegree++;
    return 0;
}
// use to check result of strdup, malloc etc.
void check (void *memory, char *message) {
  if (memory == NULL) {
    fprintf (stderr, "Can't allocate memory for %s\n", message);
    exit (3);
  }
}
int read_graph (Graph *mygraph, char *filename)
/*
 * Reads in graph from FILE *filename which is of .gx format.
 * Stores it as Graph in *mygraph.
 * Returns an error if file does not start with MAX command,
 * or if any subsequent line is not a NODE or EDGE command.
 * Does not check that node numbers do not exceed the maximum number
 * Defined by the MAX command.
 * 8/2/2010 - JLS
 */
{
  FILE *fp;
  char command[80], name[80];
  int i, s, t;
  fp= fopen (filename, "r");
  if (fp==NULL) {
    fprintf(stderr,"cannot open file %s\n", filename);
    return -1;
  }
  printf ("Reading graph from %s\n", filename);
  fscanf (fp,"%s", command);
  if (strcmp (command, "MAX")!=0) {
    fprintf (stderr, "Error in graphics file format\n");
    return -1;
  }
  else {
    fscanf (fp, "%d", &i);
    initialize_graph (mygraph, i+1); // +1 so nodes can be numbered 1..MAX
    while (fscanf (fp, "%s", command)!=EOF) {
      if (strcmp (command, "NODE")==0) {
        fscanf (fp, "%d %s", &i, name);
        insert_graph_node (mygraph, i, name);
      }
      else {
        if (strcmp (command, "EDGE")==0) {
          fscanf (fp, "%d %d", &s, &t);
          insert_graph_link (mygraph, s, t);
        }
        else {
          return -1;
        }
      }
    }
  }
  fclose(fp);
  return 0;
}
void print_graph (Graph *mygraph){
/*
 * Prints out Graph *mygraph to the stdout in .gx format - JLS
 */
  int i;
  List *current;
  printf ("MAX %d\n", mygraph->MaxSize);
  for (i=0; i<mygraph->MaxSize; i++)
    if (mygraph->table[i].name!=NULL) {
      printf ("NODE %d %s\n", i, mygraph->table[i].name);
      current= mygraph->table[i].outlist;
      while (current!=NULL) {
        printf ("EDGE %d %d\n", i, current->index);
        current= current->next;
      }
    }
}

void destroyGraph(Graph *mygraph){
    for(short i=1; i<mygraph->MaxSize; i++){
        free(mygraph->table[i].name);
        List *p = mygraph->table[i].outlist;
        while(p){
            mygraph->table[i].outlist = mygraph->table[i].outlist->next;
            free(p);
            p = mygraph->table[i].outlist;
        }
    }
    free(mygraph->table);
}