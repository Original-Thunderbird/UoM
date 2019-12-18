#include <iostream>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>

using namespace std;

typedef struct linkedlist { // linked list of ints (for use in Node)
    int index;
    struct linkedlist *next;
} List;

typedef struct{
	List *frnt, *rear;
    unsigned int siz;
}Queue;     //queue for BFS

int EmptyQueue(Queue *Q){
	if(Q->frnt == NULL && Q->rear == NULL) return 1;  //empty
	else return 0;
}

void InQueue(Queue *Q, int e){     //insert e into end of Q
	List *p = (List*)malloc(sizeof(List));
	if(!p)		exit(-1);
	p->index = e;
	p->next = NULL;
	if(Q->frnt==NULL){     //inserting the first num
        Q->frnt=p;     Q->rear = p;
	}
	else{   //inserting other nums
        Q->rear->next = p;      Q->rear = p;
	}
	Q->siz++;
}

void OutQueue(Queue *Q, int *e){     //delete the first num from Q
	List *p = Q->frnt;
	if(Q->frnt==NULL){
        printf("queue empty\n");    return ;
	}
	*e = Q->frnt->index;
	Q->frnt = Q->frnt->next;
	if(Q->frnt == NULL)    //deleting the last num
		Q->rear = NULL;
	free(p);
	Q->siz--;
}

int main(int argc, char *argv[]){
    unsigned short interval, QSIZE, CPU_NUM, i,end_run=0;
    int deal_time,total_quest=0,wait,rejected_quest=0,dealt_quest=0;
    double sum_size=0,response_time=0,pos;
    int pre;

    sscanf(argv[1], "%hu", &QSIZE);
    sscanf(argv[2], "%hu", &interval);
    sscanf(argv[3], "%hu", &CPU_NUM);
    sscanf(argv[4], "%u", &deal_time);
    unsigned int lbound = interval-interval/4, hbound = interval+interval/4, count=0;
    printf("%hd %hd %hd %u\n", QSIZE, interval, CPU_NUM, deal_time);
    //queue<int> Q;
    Queue Q;
    Q.frnt=Q.rear=NULL;
    Q.siz=0;
    int* CPUArray = (int*)calloc(CPU_NUM, sizeof(int));


    clock_t start,end;
    srand((unsigned)time(NULL));
    start = clock();  //start timing

    for(int t=1; t<=1000000; t++){
        pos = rand()%100000/100000.0;

        if(pos<1.0/interval){
            total_quest++;
            //if(Q.size()<QSIZE){
            if(Q.siz<QSIZE){
                //Q.push(t);
                InQueue(&Q,t);
            }
            else{
                rejected_quest++;
            }
        }

        //only one idle CPU accept new quest
        for(i=0; i<CPU_NUM; i++){
            if(CPUArray[i]==0  && EmptyQueue(&Q)==false){
            //if(CPUArray[i]==0  && Q.empty()==false){
                //add up response time
                //response_time += t+deal_time-Q.front();
                //Q.pop();
                OutQueue(&Q, &pre);
                response_time += (t+deal_time-pre);
                CPUArray[i] = deal_time;
                //break;
            }
        }

        //busy CPU deal with quest
        for(i=0; i<CPU_NUM; i++)
            if(CPUArray[i]>0){
                CPUArray[i]--;
                if(CPUArray[i]==0)
                    dealt_quest++;
            }

        //sum_size += Q.size();
        sum_size += Q.siz;
    }
    end = clock();  //end timing
    printf("time=%f\n",(double)(end-start)/CLOCKS_PER_SEC);
    for(i=0;i<CPU_NUM;i++)
        if(CPUArray[i]>0)
            end_run++;

    printf("Total requests: %d\n", total_quest);
    printf("Rejected requests: %d\n", rejected_quest);
    printf("Percentage of requests that was rejected: %lf\n", (double)rejected_quest/total_quest*100);
    printf("Average queue size: %lf\n", sum_size*1.0/1000000);
    printf("Average response time: %f\n", response_time*1.0/dealt_quest);
    printf("Requests Completed at the end of the simulation: %u\n", dealt_quest);
    printf("Requests Running at the end of the simulation: %hu\n",end_run);
    printf("Queue length at the end of simulation: %u\n",Q.siz);
}
