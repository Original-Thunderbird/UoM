#include <stdio.h>
#include <string.h>
#include <stdlib.h>

union Info{
  char programme_name[30];
  char room_number[30];
  char neither[30];
};
//enum type{STAFF=1,STUDENT=2,NEITHER=0};
typedef struct person{
  char name[50];
  int age;
  enum type{STAFF=1,STUDENT=2,NEITHER=0} staff_or_student;
  union Info extraInfo;
  struct person* next;
  struct person* pre;
}person;

#define CHECK 1

extern person* insert_start(person *prsn, char *name, int age, int type, char*info);
extern person* insert_end(person *prsn, char *name, int age, int type, char*info);
extern person* insert_sorted(person *prsn, char *name, int age, int type, char*info, int(*pa)(person*, person*));
extern int compare_people_by_name(person* prsn1,person* prsn2);
extern int compare_people_by_age(person* prsn1,person* prsn2);
/*
void search(person *prsn, char* name);
person* delete(person *prsn, char* name);
*/
