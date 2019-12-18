#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct BiTNode
{
	char *obName;
	char *question;
	struct BiTNode *yes, *no;
}BiTNode, *BitTree;

void nodePrint(BiTNode *L);



void nodePrint(BiTNode *L){
  printf("Object:");
  if(L->obName==NULL){
    printf("NOTHING\n");
  }
  else{
    printf("%s\n", L->obName);
  }
  printf("Question:");
  if(L->question==NULL){
    printf("NOTHING\n");
  }
  else{
    printf("%s\n", L->question);
  }
	//print question/object name for yes/no
  if(L->yes){
		printf("Yes: ");
		if(L->yes->obName){
			printf("a %s\n", L->yes->obName);
		}
		else if(L->yes->question){
			printf("a %s\n", L->yes->question);
		}
		else ;
  }
	if(L->no){
		printf("Yes: ");
		if(L->no->obName){
			printf("a %s\n", L->no->obName);
		}
		else if(L->no->question){
			printf("a %s\n", L->no->question);
		}
		else ;
  }
}

void treePrint(BiTNode *L){
  //PreOrderTraverse
	if (L == NULL)
		return ;
	nodePrint(L);
	if (L->yes != NULL)
		treePrint(L->yes);
	if (L->yes != NULL)
		treePrint(L->no);
}

void DestroyBiTree(BiTNode *L){
	//destroy childen first, the node itself later(preorderTraverse)
	if (L==NULL)		return ;
	if (L->yes != NULL)
        DestroyBiTree(L->yes);
	if (L->no != NULL)
        DestroyBiTree(L->no);
	if(L->obName)
		free(L->obName);
	if(L->question)
		free(L->question);
	free(L);
	return ;
}

int main(){
	//hardcode
	//L
	BiTNode *L = (BiTNode*)malloc(sizeof(BiTNode));
	L->obName=NULL;
	L->question=(char*)malloc(40*sizeof(char));
	strcpy(L->question,"Does it have a tail?");
	//L->yes
	L->yes=(BiTNode*)malloc(sizeof(BiTNode));
	L->yes->obName=(char*)malloc(40*sizeof(char));
	L->yes->question=NULL;
	L->yes->yes=NULL;
	L->yes->no=NULL;
	//L->no
	strcpy(L->yes->obName,"pangolin");
	L->no=(BiTNode*)malloc(sizeof(BiTNode));
	L->no->obName=NULL;
	L->no->question=(char*)malloc(40*sizeof(char));
	strcpy(L->no->question,"Is it flat, round and edible?");
	//L->no->yes
	L->no->yes=(BiTNode*)malloc(sizeof(BiTNode));
	L->no->yes->obName=NULL;
	L->no->yes->question=(char*)malloc(48*sizeof(char));
	strcpy(L->no->yes->question,"Can you dip it in your tea?");
	L->no->yes->yes=(BiTNode*)malloc(sizeof(BiTNode));
	//L->no->yes->yes
	L->no->yes->yes->obName=(char*)malloc(40*sizeof(char));
	L->no->yes->yes->question=NULL;
	L->no->yes->yes->yes=NULL;
	L->no->yes->yes->no=NULL;
	strcpy(L->no->yes->yes->obName,"a biscuit");
	//L->no->yes->no
	L->no->yes->no=(BiTNode*)malloc(sizeof(BiTNode));
	L->no->yes->no->obName=(char*)malloc(40*sizeof(char));
	L->no->yes->no->question=NULL;
	L->no->yes->no->yes=NULL;
	L->no->yes->no->no=NULL;
	strcpy(L->no->yes->no->obName,"a pizza");
	//L->no->no
	L->no->no=(BiTNode*)malloc(sizeof(BiTNode));
	L->no->no->obName=(char*)malloc(5*sizeof(char));
	L->no->no->question=NULL;
	strcpy(L->no->no->obName,"Pete");
	L->no->no->yes=NULL;
	L->no->no->no=NULL;
	treePrint(L);
	//clean memory
	DestroyBiTree(L);
	return 0;
}
