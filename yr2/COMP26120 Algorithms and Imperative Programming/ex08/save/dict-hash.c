#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define __USE_BSD
#include <string.h>

#include "speller.h"
#include "dict.h"
#define ADDSIZE 500000

typedef struct { // hash-table entry
  Key_Type element; // only data is the key itself
  enum {empty, in_use, deleted} state;
} cell;

typedef unsigned int Table_size; // type for size-of or index-into hash table

struct table {
  cell *cells;
  Table_size table_size; // cell cells [table_size];
  Table_size num_entries; // number of cells in_use
  unsigned short collision;
  // add anything else that you need
};

typedef struct table TNode;

unsigned APHash(Key_Type key, unsigned mod);
unsigned polynomialHash(Key_Type key, unsigned mod);
Table rehash(Table T);

Table initialize_table (Table_size initsize) {
    Table T=(Table)malloc(sizeof(TNode));
    T->cells=(cell*)calloc(initsize,sizeof(cell));  //everything in array initialized as 0
    T->num_entries=0; //number of keys
    T->table_size=initsize;
    T->collision=0;  //collision counting
    return T;
}

Table insert (Key_Type key, Table T) {
    unsigned hashAddress=0;
    if(mode==1){    //APHash+linear
        hashAddress=APHash(key, T->table_size);
        while(T->cells[hashAddress].state!=empty){
            if(!strcmp(T->cells[hashAddress].element,key)&&T->cells[hashAddress].state==in_use){
                //printf("key collision:%s\n",key);
                return T;
            }
            hashAddress=(hashAddress+1)%T->table_size;  //linear
            T->collision++;
        }
    }
    else if(mode==2){   //polynomial+quadratic
        hashAddress=polynomialHash(key, T->table_size);
        char j=1;
        while(T->cells[hashAddress].state!=empty){
            if(!strcmp(T->cells[hashAddress].element,key)&&T->cells[hashAddress].state==in_use){
                //printf("key collision:%s\n",key);
                return T;
            }
            hashAddress=(hashAddress+j*j-(j-1)*(j-1))%T->table_size;  //quadratic
            T->collision++;     j++;
        }
    }
    else if(mode==3){   //double hashing
        hashAddress=APHash(key, T->table_size);
        unsigned adder=polynomialHash(key, T->table_size);
        while(T->cells[hashAddress].state!=empty){
            if(!strcmp(T->cells[hashAddress].element,key)&&T->cells[hashAddress].state==in_use){
                //printf("key collision:%s\n",key);
                return T;
            }
            hashAddress=(hashAddress+adder)%T->table_size;  //index increase by adder
            T->collision++;
        }
    }
    //insert new key & change node state
    T->cells[hashAddress].element=(Key_Type)malloc(strlen(key)+1);
    strcpy(T->cells[hashAddress].element, key);
    T->cells[hashAddress].state=in_use;
    T->num_entries++;
    //rehash when load factor >0.6
    if(T->num_entries*1.0/T->table_size>0.6){
        T=rehash(T);
    }
    return T;
}

Boolean find (Key_Type key, Table T){
    if(mode==1){ //APHash+linear
        unsigned hashAddress=APHash(key,T->table_size),save=hashAddress;
        if (T->cells[hashAddress].state==empty) {
            //printf("%s not found\n",key);
            return FALSE;
        }
        //collision:linear addressing
        while(strcmp(T->cells[hashAddress].element,key)) {
            hashAddress=(hashAddress+1)%T->table_size;
            if (T->cells[hashAddress].state==empty||hashAddress==save) {
                //printf("%s not found\n",key);
                return FALSE;
            }
        }
        //printf("%s found at %u\n",key, hashAddress);
        return TRUE;
    }

    else if(mode==2){  //polynomial+quadratic
        unsigned hashAddress=polynomialHash(key, T->table_size);
        if (T->cells[hashAddress].state==empty) {
            //printf("%s not found\n",key);
            return FALSE;
        }
        char j=1;
        //collision:quadratic addressing
        while(strcmp(T->cells[hashAddress].element,key)) {
            hashAddress=(hashAddress+j*j-(j-1)*(j-1))%T->table_size;
            if (T->cells[hashAddress].state==empty||j>=T->table_size) {
                //printf("%s not found\n",key);
                return FALSE;
            }
            j++;
        }
        //printf("%s found at %u\n",key, hashAddress);
        return TRUE;
    }
    else if(mode==3){  //double hashing
        unsigned adder=polynomialHash(key, T->table_size), hashAddress=APHash(key, T->table_size),j=0;
        if (T->cells[hashAddress].state==empty) {
            //printf("%s not found\n",key);
            return FALSE;
        }
        //collision:adder addressing
        while(strcmp(T->cells[hashAddress].element,key)){
            hashAddress=(hashAddress+adder)%T->table_size;
            if (T->cells[hashAddress].state==empty||j>=T->table_size) {
                //printf("%s not found\n",key);
                return FALSE;
            }
            j++;
        }
        //printf("%s found at %u\n",key, hashAddress);
        return TRUE;
    }
    return TRUE;
}

void print_table (Table T){

    for (unsigned int i=0;i<T->table_size;i++) {
        if(T->cells[i].state==in_use)
            printf("%d:%s\n",i,T->cells[i].element);
    }

}

void print_stats (Table T){
    printf("total entries:%u\nused entries:%u\n",T->table_size, T->num_entries);
    printf("load factors:%f\ncollisions:%hu\n",T->num_entries/T->table_size, T->collision);
}

unsigned APHash(Key_Type key, unsigned mod){
    //utilize as much bits as possible while making hash value unique
    unsigned u=0;
    for(unsigned short i=0;i<strlen(key);i++)
        u^=(u<<5)+key[i]+(u>>2);
    return u%mod;
}

unsigned polynomialHash(Key_Type key, unsigned mod){
    //trditional & easy to implement
    unsigned u=0,base=37;
    for(unsigned short i=0;i<strlen(key);i++)
        u=u*base+(key[i]-'a');
    return u%mod;
}

Table rehash(Table T){
    cell *curr;
    /* Setup the new elements */
    cell *temp = (cell *)calloc(T->table_size+ADDSIZE, sizeof(cell));
    if (!temp) {
        printf("memory allocation error, stay at original map\n");
        return T;
    }

    /* Update the array */
    curr = T->cells;
    T->cells = temp;

    /* Update the size */
    unsigned old_size=T->table_size;
    T->table_size += ADDSIZE;
    T->collision = T->num_entries = 0;

    /* Rehash the elements */
    for (unsigned i=0;i<old_size;i++){
        if ((curr+i)->state == empty) {
            continue;
        }
        T = insert((curr+i)->element, T);
    }
    free(curr);
    return T;
}

void tidy(Table T){
    for(unsigned i=0;i<T->table_size;i++){
        if(T->cells[i].element!=NULL){
            free(T->cells[i].element);
        }
    }
    free(T->cells);
    free(T);
}
