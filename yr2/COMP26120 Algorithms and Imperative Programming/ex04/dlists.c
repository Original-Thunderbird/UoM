#include "dlists.h"

person* insert_start(person *prsn, char *name, int age, int type, char *info)
{
  person *new= (person*)malloc(sizeof(person));
  if(new!=NULL){
     strcpy(new->name,name);
     new->age=age;
     new->next=prsn;
     if(prsn){
       prsn->pre=new;
     }
     new->pre=NULL;
     new->staff_or_student=type;
     if(new->staff_or_student==STUDENT){
       strcpy(new->extraInfo.programme_name,info);
     }
     else if(new->staff_or_student==STAFF){
       strcpy(new->extraInfo.room_number,info);
     }
     else{
       strcpy(new->extraInfo.neither,info);
     }
     prsn=new;
     return prsn;
  }
  else{
     printf("EXITCODE is not 0");
     return NULL;
  }
}

person* insert_end(person *prsn, char *name, int age, int type, char*info)
{
  person *q,*new= (person*)malloc(sizeof(person));
  if(new!=NULL){
     strcpy(new->name,name);
     new->age=age;
     new->staff_or_student=type;
     if(new->staff_or_student==STUDENT){
       strcpy(new->extraInfo.programme_name,info);
     }
     else if(new->staff_or_student==STAFF){
       strcpy(new->extraInfo.room_number,info);
     }
     else{
       strcpy(new->extraInfo.neither,info);
     }
     if(!prsn){
        new->pre=NULL;
	      new->next=prsn;
        prsn->pre=new;
        prsn=new;
     }
     else{
	      for(q=prsn;q->next;q=q->next);
        new->next=NULL;
        q->next=new;
        new->pre=q;
     }
     return prsn;
  }
  else{
     printf("EXITCODE is not 0");
     return NULL;
  }
}

person* insert_sorted(person *prsn, char *name, int age, int type, char*info, int(*pa)(person*, person*))
{
  person *q,*p,*new= (person*)malloc(sizeof(person));
  if(new!=NULL){
     strcpy(new->name,name);
     new->age=age;
     new->staff_or_student=type;
     if(new->staff_or_student==STUDENT){
       strcpy(new->extraInfo.programme_name,info);
     }
     else if(new->staff_or_student==STAFF){
       strcpy(new->extraInfo.room_number,info);
     }
     else{
       strcpy(new->extraInfo.neither,info);
     }
     q=prsn;
     if(!prsn||pa(q,new)>0){
        new->pre=NULL;
        new->next=prsn;
        prsn->pre=new;
        prsn=new;
     }
     else{
      	for(p=q->next;p;q=q->next,p=p->next){
      	    if(pa(p,new)>0){
            		new->next=p;
                p->pre=new;
            		q->next=new;
                new->pre=q;
            		break;
            }
        }
      	if(!p){
      	    new->next=p;
      	    q->next=new;
            new->pre=q;
      	}
     }
     return prsn;
  }
  else{
     printf("EXITCODE is not 0");
     return NULL;
  }
}
/*
void search(person *prsn, char* name){
  person *q;
  for(q=prsn; q; q=q->next){
    if(!strcmp(q->name,name)){
      printf("%s %d\n",q->name, q->age);
    }
  }
  putchar('\n');
}

person* delete(person *prsn, char* name){
  if(prsn==NULL){
    return NULL;
  }
  person *q;
  for(q=prsn; q; q=q->next){
    if(!strcmp(q->name,name)){
      if(q==prsn){
        prsn=prsn->next;
        free(prsn->pre);
        prsn->pre=NULL;
      }
      else if(q->next==NULL){
        q->pre->next=NULL;
        free(q);
      }
      else{
        q->pre->next=q->next;
        q->next->pre=q->pre;
        free(q);
      }
    }
  }
  for(q=prsn; q; q=q->next){
    printf("%s %d\n",q->name, q->age);
  }
  return prsn;
}
*/
