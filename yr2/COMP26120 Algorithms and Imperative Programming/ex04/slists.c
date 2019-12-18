#include "slists.h"

person* insert_start(person *prsn, char *name, int age, int type, char *info)
{
  person *new= (person*)malloc(sizeof(person));
  if(new!=NULL){
     strcpy(new->name,name);
     new->age=age;
     new->next=prsn;
     new->staff_or_student=type;
     if(new->staff_or_student==STUDENT){  /*depends on type*/
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
	new->next=prsn;
        prsn=new;
     }
     else{
	for(q=prsn;q->next;q=q->next);
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
	      new->next=prsn;
        prsn=new;
     }
     else{
      	for(p=q->next;p;q=q->next,p=p->next){
      	    if(pa(p,new)>0){
            		new->next=p;
            		q->next=new;
            		break;
            }
        }
      	if(!p){
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
