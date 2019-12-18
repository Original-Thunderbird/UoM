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

void saveTree(BiTNode *L, FILE *out);
void loadTree(BiTNode **L, FILE *in);
void initialize(BiTNode **L);
void insertQuestion(BiTNode *p, char* obj, char* qst, char* ans);
void DestroyBiTree(BiTNode *L);
void treePrint(BiTNode *L);
void nodePrint(BiTNode *L);
char* toLower(char* str);


int main(){
    char ans[4],jud[4],buf[82],obj[80],qst[80],filename[80];
    BiTNode *L=NULL, *p;
    printf("do you want to read your saved file?(yes/no)\n");
    scanf("%s", jud);  getchar();
    if(!strcmp(toLower(jud),"yes")){
			//read file
        printf("input target file name:\n");
        scanf("%s", filename);  getchar();
        FILE *in=fopen(filename,"r");
        if(in==NULL){
            return -1;
        }
        loadTree(&L,in);
        L->parent=NULL;
        fclose(in);
    }
    else{
			//for pangolin node
        initialize(&L);
    }
		do{
        p=L;
        while(p){
            if(!(p->yes)&&!(p->no)){
                printf("Is it %s?\n",p->obName);
                scanf("%s",ans);    getchar();
								//insertion handling for judgement(correct,absolutely,y,capitalization)
                if(toLower(ans)[0]=='y' || !strcmp(toLower(ans),"correct")|| !strcmp(toLower(ans),"absolutely")){
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
                    printf("What is the answer for %s?(yes/no)\n", obj);
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
        printf("Would you like to play again?(yes/no)\n");
        scanf("%s",jud); getchar();
    }while(!strcmp(toLower(jud),"yes"));
    printf("do you want to save your tree?(yes/no)\n");
		//save file
    scanf("%s", jud);  getchar();
    if(!strcmp(toLower(jud),"yes")){
        printf("input target file name:\n");
        scanf("%s", filename);  getchar();
        FILE *out=fopen(filename,"w");
        saveTree(L,out);
        fclose(out);
    }
    DestroyBiTree(L);
}

void saveTree(BiTNode *L, FILE *out){
    //preorderTraverse
	if (L == NULL)
		return ;
    else{
        if(L->question){
					//not leaf node(question)
            fprintf(out,"question:%s\n",L->question);
            saveTree(L->yes, out);
            saveTree(L->no, out);
        }
        else{
					//leaf node(object)
            fprintf(out,"object:%s\n",L->obName);
        }
    }
}

void loadTree(BiTNode **L, FILE *in){
    char buf[100];
    fgets(buf, 100, in);
    if(!strlen(buf)){
        return NULL;
    }
    else{
			//not leaf node(question)
        (*L)=(BiTNode*)malloc(sizeof(BiTNode));
        if(strstr(buf,"question")){
            (*L)->obName=NULL;
            buf[strlen(buf)-1]=0;
            (*L)->question=(char*)malloc((strlen(buf)-8)*sizeof(char));
            strcpy((*L)->question,buf+9);
            loadTree(&(*L)->yes, in);
            (*L)->yes->parent=(*L);
            loadTree(&(*L)->no, in);
            (*L)->no->parent=(*L);
        }
        else{
					//leaf node(object)
            buf[strlen(buf)-1]=0;
            (*L)->obName=(char*)malloc((strlen(buf)-6)*sizeof(char));
            strcpy((*L)->obName,buf+7);
            (*L)->question=NULL;
            (*L)->yes=NULL;
            (*L)->no=NULL;
        }
    }
}

void initialize(BiTNode **L){
	//node pangolin
    (*L)=(BiTNode*)malloc(sizeof(BiTNode));
    (*L)->obName=(char*)malloc(11*sizeof(char));
    strcpy((*L)->obName,"a pangolin");
    (*L)->question=NULL;
    (*L)->yes=NULL;
    (*L)->no=NULL;
    (*L)->parent=NULL;
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

//switch a whole word to lower case
char* toLower(char* str){
    for(int i=0; i<strlen(str); i++){
        if(isalpha(str[i])){
            str[i]=tolower(str[i]);
        }
    }
    return str;
}
