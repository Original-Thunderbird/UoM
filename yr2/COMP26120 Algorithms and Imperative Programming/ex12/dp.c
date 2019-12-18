
// Dynamic programming for 0/1 knapsack
// (C) Joshua Knowles
// The variable names here follow standard usage in formulating
// dynamic programming via recursive definitions

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include<time.h>

FILE *fp;  // file pointer for reading the input files
int Capacity;     // capacity of the knapsack (total weight that can be stored)
int Nitems;    // number of items available
int *item_weights;  // vector of item weights
int *item_values;  // vector of item profits or values
int *temp_indexes;  // list of temporary item indexes for sorting items by value/weight
int QUIET=0; // this can be set to 1 to suppress output


// function prototypes
extern void read_knapsack_instance(char *filename);
int DP(int *v,int *wv, int n, int W, int *solution);
extern int check_evaluate_and_print_sol(int *sol,  int *total_value, int *total_weight);

int main(int argc, char *argv[1]){
    int *solution;    // binary vector indicating items to pack
    int total_value, total_weight;  // total value and total weight of items packed
    read_knapsack_instance(argv[1]);

    if((solution = (int *)malloc((Nitems+1)*sizeof(int)))==NULL){
        fprintf(stderr,"Problem allocating table for DP\n");
        exit(1);
    }
    memset(solution,0,(Nitems+1)*sizeof(int));
    clock_t start,end;
    start = clock();  //start timing
    DP(item_values,item_weights,Nitems,Capacity,solution);
    end = clock();
    printf("time=%f\n",(double)(end-start)/CLOCKS_PER_SEC);
    check_evaluate_and_print_sol(solution,&total_weight,&total_value);
    free(item_values);
    free(item_weights);
    free(temp_indexes);
    free(solution);
    return(0);
}

int DP(int *v,int *wv, int n, int W, int *solution){
  // the dynamic programming function for the knapsack problem
  // the code was adapted from p17 of http://www.es.ele.tue.nl/education/5MC10/solutions/knapsack.pdf

  // v array holds the values / profits / benefits of the items
  // wv array holds the sizes / weights of the items
  // n is the total number of items
  // W is the constraint (the weight capacity of the knapsack)
  // solution: a 1 in position n means pack item number n+1. A zero means do not pack it.
    // keep[][] and V[][] are both of size (n+1)*(W+1)
    int **V;
    int **keep;  // 2d arrays for use in the dynamic programming solution
    int i, w, K;

    // Dynamically allocate memory for variables V and keep
    V = (int**)malloc((n+1)*sizeof(int*));
    keep = (int**)malloc((n+1)*sizeof(int*));
    //V[0] = (int*)malloc((n+1)*(W+1)*sizeof(int));
    //keep[0] = (int*)malloc((n+1)*(W+1)*sizeof(int));
    for(i=0;i<n+1;i++){
        //V[i] = V[0] + i*(W+1);//ÿ����columns��int��Ԫ�أ�����rowsÿ��1����ַ��i * columns
        //keep[i] = keep[0] + i*(W+1);
        V[i] = (int*)malloc((W+1)*sizeof(int));
        keep[i] = (int*)calloc((W+1),sizeof(int));
    }
    //set the values of the zeroth row of the partial solutions table to zero
    for(w=0;w<=W;w++)   V[0][w]=0;
    // main dynamic programming loops , adding one item at a time and looping through weights from 0 to W
    for(i=1;i<=n;i++)
        for(w=0;w<=W;w++)
        //using optimal subproblems V[i-1,w] and v[i]+V[i-1,w-wv[i]] to construct solutions for current problem
            //the first constrain will work when current w is still too small
            if(wv[i]<=w && v[i]+V[i-1][w-wv[i]] > V[i-1][w]){
                V[i][w] = v[i] + V[i-1][w-wv[i]];
                keep[i][w] = 1;
            }
            else{
                V[i][w] = V[i-1][w];
                keep[i][w] = 0;
            }
    // now discover which items were in the optimal solution
    K=W;
    for(i=n;i>0;i--){
        if(keep[i][K]==1){
            solution[i]=1;
            K = K-wv[i];
        }
    }
    return V[n][W];
}
