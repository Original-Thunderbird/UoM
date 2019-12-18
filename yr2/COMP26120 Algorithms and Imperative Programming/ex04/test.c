#ifdef DOUBLE
#include "dlists.h"
#else
#include "slists.h"
#endif
#include "data.h"
#include "printing.h"

int compare_people_by_name(person* prsn1,person* prsn2){
  return strcmp(prsn1->name,prsn2->name);
}

int compare_people_by_age(person* prsn1,person* prsn2){
  return prsn1->age-prsn2->age;
}

int main(int argc, char **argv)
{
  short i;
  person *people=NULL,*p,*q;
  if(argc==2&&!strcmp(argv[1],"insert_end")){
    for (i=0; i<HOW_MANY; i++) {
      people=insert_end(people, names[i], ages[i], person_type[i], info[i]);
    }
  }
  else if(argc==1||(argc==2&&!strcmp(argv[1],"insert_start"))){
    for (i=0; i<HOW_MANY; i++){
      people=insert_start(people, names[i], ages[i], person_type[i], info[i]);
    }
  }
  else if(!strcmp(argv[1],"insert_sorted")){
    if(argc==2||(argc==3&&!strcmp(argv[2],"name"))){
      for (i=0; i<HOW_MANY; i++){
        people=insert_sorted(people, names[i], ages[i], person_type[i], info[i], compare_people_by_name);
      }
    }
    else if(argc==3&&!strcmp(argv[2],"age")) {
      for (i=0; i<HOW_MANY; i++){
        people=insert_sorted(people, names[i], ages[i], person_type[i], info[i], compare_people_by_age);
      }
    }
  }
  else{
    perror("argumenbt fault\n");
    return -1;
  }
#ifdef DOUBLE
  for (q=people; q->next; q=q->next);
  for(;q;q=q->pre){
    printData(q);
  }
#else
  for (q=people; q; q=q->next) {
    printData(q);
  }
#endif
/*
#ifdef CHECK
  char name[30];
  printf("input name:");
  scanf("%s", name);  getchar();
  search(people,name);
  people=delete(people,name);
#endif
*/
  for (q=people,p=q->next; p; p=p->next) {
    free(q);
    q=p;
  }
  free(q);
  q=NULL;

  return 0;
}
