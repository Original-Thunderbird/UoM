#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arrays.h"

typedef struct person{
  char name[50];
  int age;
}person;

static void insert(person **prsn, char *name, int age, int *nextfreeplace) 
{
  *prsn= (person*)malloc(sizeof(person));
  if(prsn!=NULL){  
     strcpy((*prsn)->name,name);
     (*prsn)->age=age;
     (*nextfreeplace)++;
  }
  else{
     printf("EXITCODE is not 0");
  }
}

int main(int argc, char **argv) 
{
  int nextfreeplace = 0;
  short i;
  person *people[HOW_MANY];

  for (i=0; i<HOW_MANY; i++) 
  {
    insert (&people[i], names[i], ages[i], &nextfreeplace);
  }
  for (i=0; i<HOW_MANY; i++) {
    printf("%d: %s is %d\n",i,people[i]->name,people[i]->age);
  }

  for (i=0; i<nextfreeplace; i++) {
    free(people[i]);
  }
  return 0;
}
