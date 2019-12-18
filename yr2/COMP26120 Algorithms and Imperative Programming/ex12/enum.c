
// enum - full enumeration of knapsack solutions
// (C) Joshua Knowles

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

FILE *fp;  // file pointer for reading the input files
int Capacity;     // capacity of the knapsack (total weight that can be stored)
int Nitems;    // number of items available
int *item_weights;  // vector of item weights
int *item_values;  // vector of item profits or values
int *temp_indexes;  // list of temporary item indexes for sorting items by value/weight
int QUIET=0; // this can be set to 1 to suppress output

extern void read_knapsack_instance(char *filename);
extern void print_instance();
extern void sort_by_ratio();
extern int check_evaluate_and_print_sol(int *sol,  int *total_value, int *total_weight);
void enumerate();
int next_binary(int *str, int Nitems);

int main(int argc, char *argv[]){
    read_knapsack_instance(argv[1]);
    print_instance();
    enumerate();
    free(item_values);
    free(item_weights);
    free(temp_indexes);
    return(0);
}

//code for updating trckbar
void update_process(int percent,int barlen){
    int i;
    putchar('[');
    for(i=1;i<=barlen;++i)
    putchar(i*100<=percent*barlen?'>':' ');
    putchar(']');
    printf("%3d%%",percent);
    for(i=0;i!=barlen+6;++i)
    putchar('\b');
}

void enumerate(){
    // Do an exhaustive search (aka enumeration) of all possible ways to pack the knapsack.
    // This is achieved by creating every binary solution vector of length Nitems. For each solution vector, its value and weight is calculated.
    int i;  // item index
    int solution[Nitems+1];   // (binary) solution vector representing items packed
    int best_solution[Nitems+1];  // (binary) solution vector for best solution found
    int best_value=0,record_weight=0; // total value packed in the best solution
    int total_value, total_weight,count=0; // total value and total weight of current knapsack solution
    int infeasible;  // 0 means feasible; -1 means infeasible (violates the capacity constraint)
    // set the knapsack initially empty
    memset(solution,0,(Nitems+1)*sizeof(int));

    QUIET=1;
    float sum,divisor = pow(2,Nitems);

    while(!(next_binary(&solution[1], Nitems))){
        /* ADD CODE IN HERE TO KEEP TRACK OF FRACTION OF ENUMERATION DONE */
        sum = 0;
        for(i=1;i<=Nitems;i++){
            if(solution[i])
                sum+=pow(2,21-i);
        }
        count++;
        if(!(count%10240)){
            //update trackbar, but not that frequent
            update_process(sum*50/divisor,50);
        }
        // calculates the value and weight and feasibility:
        infeasible=check_evaluate_and_print_sol(solution, &total_value, &total_weight);
        /* ADD CODE IN HERE TO KEEP TRACK OF BEST SOLUTION FOUND*/
        if(!infeasible&&best_value<total_value){
            //new best solution found
            memcpy(best_solution,solution,(Nitems+1)*sizeof(int));
            best_value = total_value;
            record_weight = total_weight;
        }
    }
    /* ADD CODE TO PRINT OUT BEST SOLUTION */
    update_process(100,50);
    putchar('\n');
    for(i=1;i<=Nitems;i++){
        if(best_solution[i]){//items in best solution pack
            printf("%d\t%d\t%d\n",temp_indexes[i],item_weights[i],item_values[i]);
        }
    }
    printf("total value:%d\t total weight:%d",best_value,record_weight);
}

int next_binary(int *str, int Nitems){
  // Called with a binary string of length Nitems, this
  // function adds "1" to the string, e.g. 0001 would turn to 0010.
  // If the string overflows, then the function returns 1, else it returns 0.
    int i=Nitems-1;
    while(i>=0){
        if(str[i]==1){
            str[i]=0;
            i--;
        }
        else{
            str[i]=1;
            break;
        }
    }
    if(i==-1)     return(1);
    else          return(0);
}
