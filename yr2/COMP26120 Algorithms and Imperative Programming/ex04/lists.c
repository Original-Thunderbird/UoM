#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"

typedef struct person{
  char name[50];
  int age;
  struct person* next;
}person;

int compare_people_by_name(person* prsn1,person* prsn2){
  return strcmp(prsn1->name,prsn2->name);
}

int compare_people_by_age(person* prsn1,person* prsn2){
  return prsn1->age-prsn2->age;
}

static person* insert_start(person *prsn, char *name, int age)
{
  person *new= (person*)malloc(sizeof(person));
  if(new!=NULL){
     strcpy(new->name,name);
     new->age=age;
     new->next=prsn;
     prsn=new;
     return prsn;
  }
  else{
     printf("EXITCODE is not 0");
     return NULL;
  }
}

static person* insert_end(person *prsn, char *name, int age)
{
  person *q,*new= (person*)malloc(sizeof(person));
  if(new!=NULL){
     strcpy(new->name,name);
     new->age=age;
     if(!prsn){
	      new->next=prsn;
        prsn=new;
     }
     else{
      	for(q=prsn;q->next;q=q->next);   /*to tail*/
      	new->next=q->next;
        q->next=new;
	      q=new;
     }
     return prsn;
  }
  else{
     printf("EXITCODE is not 0");
     return NULL;
  }
}

static person* insert_sorted(person *prsn, char *name, int age, int(*pa)(person*, person*))
{
  person *q,*p,*new= (person*)malloc(sizeof(person));
  if(new!=NULL){
     strcpy(new->name,name);
     new->age=age;
     q=prsn;
     if(!prsn||pa(q,new)>0){  /*head*/
	      new->next=prsn;
        prsn=new;
     }
     else{
      	for(p=q->next;p;q=q->next,p=p->next){  /*middle*/
      	    if(pa(p,new)>0){
            		new->next=p;
            		q->next=new;
            		break;
            }
        }
      	if(!p){  /*tail*/
      	    new->next=p;
      	    q->next=new;
      	}
     }
     return prsn;
  }
  else{
     printf("EXITCODE is not 0");
     return NULL;
  }
}

int main(int argc, char **argv)
{
  short i;
  person *people=NULL,*p,*q;
  if(argc==2&&!strcmp(argv[1],"insert_end")){
    for (i=0; i<HOW_MANY; i++) {
      people=insert_end(people, names[i], ages[i]);
    }
  }
  else if(argc==1||(argc==2&&!strcmp(argv[1],"insert_start"))){
    for (i=0; i<HOW_MANY; i++){
      people=insert_start(people, names[i], ages[i]);
    }
  }
  else if(!strcmp(argv[1],"insert_sorted")){
    if(argc==2||(argc==3&&!strcmp(argv[2],"name"))){
      for (i=0; i<HOW_MANY; i++){
        people=insert_sorted(people, names[i], ages[i], compare_people_by_name);
      }
    }
    else if(argc==3&&!strcmp(argv[2],"age")) {
      for (i=0; i<HOW_MANY; i++){
        people=insert_sorted(people, names[i], ages[i], compare_people_by_age);
      }
    }
  }
  else{
    perror("EXITCODE is not 0\n");
    return -1;
  }
  for (q=people; q; q=q->next) { /*print*/
    printf("Name: %s, age: %d\n",q->name,q->age);
  }
  /*
  q=people;
  while (q) {
    p=q;
    q=q->next;
    free(p);
  }
  */

    for (q=people,p=q->next; p; p=p->next) {  /*free*/
      free(q);
      q=p;
    }
    free(q);
    q=NULL;

  return 0;
}
