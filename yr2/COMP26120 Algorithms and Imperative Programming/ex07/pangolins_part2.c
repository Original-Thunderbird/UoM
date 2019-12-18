#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>

typedef struct BiTNode
{
	char *obName;
	char *question;
	struct BiTNode *yes, *no, *parent;
}BiTNode;

//switch a whole word to lower case
char* toLower (char* str){
    for(int i=0; i<strlen(str); i++){
        if(isalpha(str[i])){
            str[i]=tolower(str[i]);
        }
    }
    return str;
}

void insertQuestion(BiTNode *p, char* obj, char* qst, char* ans){
    BiTNode *parent=NULL;
    int flag=0;
		//save parent node for insertion
    if(p->parent){
        parent=p->parent;
        if(p==parent->yes){
            flag=1;
        }
    }
		//create new question node and string
    BiTNode *nroot = (BiTNode*)malloc(sizeof(BiTNode));
    nroot->obName=NULL;
    nroot->question=(char*)malloc((strlen(qst)+1)*sizeof(char));
    strcpy(nroot->question,qst);
    nroot->parent=NULL;
		//create new object node and string
    BiTNode *nnode = (BiTNode*)malloc(sizeof(BiTNode));
    nnode->obName=(char*)malloc((strlen(obj)+1)*sizeof(char));
    strcpy(nnode->obName,obj);
    nnode->question=NULL;
    nnode->yes=NULL;
    nnode->no=NULL;
    nnode->parent=nroot;
		//insert object node according to answer of the new question
    if(toLower(ans)[0]=='y' || !strcmp(toLower(ans),"correct")|| !strcmp(toLower(ans),"absolutely")){
        nroot->yes=nnode;
        nroot->no=p;
    }
    else{
        nroot->no=nnode;
        nroot->yes=p;
    }
		//insertion of question node
    p->parent=nroot;
    if(parent){
        if(flag==1){
            parent->yes=nroot;
        }
        else{
            parent->no=nroot;
        }
				//case that new node is root
        nroot->parent=parent;
    }
}

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

BiTNode* initialize(BiTNode *L){
    L=(BiTNode*)malloc(sizeof(BiTNode));
    L->obName=(char*)malloc(11*sizeof(char));
    strcpy(L->obName,"a pangolin");
    L->question=NULL;
    L->yes=NULL;
    L->no=NULL;
    L->parent=NULL;
		return L;
}

void DestroyBiTree(BiTNode *L){
	if (L==NULL)		return ;
	if (L->yes != NULL)
        DestroyBiTree(L->yes);
	if (L->no != NULL)
        DestroyBiTree(L->no);
    if (L->obName)
        free(L->obName);
    if (L->question)
        free(L->question);
	free(L);
}
int main(){
    BiTNode *L=NULL, *p;
		//for pangolin node
    L=initialize(L);
    char ans[4],jud[4],buf[82],obj[80],qst[80];
    do{
        p=L;
        while(p){
            if(!(p->yes)&&!(p->no)){
                printf("Is it %s?\n",p->obName);
                scanf("%s",ans);    getchar();
								//insertion handling for judgement(correct,absolutely,y,capitalization)
                if(toLower(ans)[0]=='y' || !strcmp(toLower(ans),"correct")|| !strcmp(toLower(ans),"absolutely") ){
                    printf("Good. That was soooo easy.\n");
                    break;
                }
                else{
                    printf("Oh. Well you win then -- What were you thinking of?\n");
                    fgets(buf, 82, stdin);
                    strcpy(obj,buf);
										//delete \n
                    obj[strlen(obj)-1]=0;
                    printf("Please give me a question about %s, so I can tell the difference between %s and %s\n",obj, obj, p->obName);
                    fgets(buf, 82, stdin);
                    strcpy(qst,buf);
                    qst[strlen(qst)-1]=0;
										//handiling for question mark losing
										if(qst[strlen(qst)-1]!='?'){
											qst[strlen(qst)]='?';
											qst[strlen(qst)]=0;
										}
                    printf("What is the answer for %s?\n", obj);
                    scanf("%s", ans);
                    printf("Thanks\n");
                    insertQuestion(p, obj, qst, ans);
										//in case new root is set
                    if(L->parent){
                        L=L->parent;
                    }
                    break;
                }
            }
            else{
							//going down the tree
                printf("%s\n",p->question);
                scanf("%s",ans);    getchar();
                if(toLower(ans)[0]=='y' || !strcmp(toLower(ans),"correct")|| !strcmp(toLower(ans),"absolutely")){
                    p=p->yes;
                }
                else{
                    p=p->no;
                }
            }
        }
        printf("Would you like to play again?\n");
        scanf("%s",jud); getchar();
    }while(toLower(jud)[0]=='y');
    DestroyBiTree(L);
}
