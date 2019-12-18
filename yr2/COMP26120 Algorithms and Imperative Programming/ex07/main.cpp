#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE -1
#define OK 1
#define ERROR -2
typedef int Status;   //�Ǻ��������ͣ���ֵ�Ǻ������״̬����
typedef int ElemType; //����Ԫ�����Ͷ���
typedef struct BiTNode
{
	ElemType key;
	char data;
	struct BiTNode *lchild, *rchild;//��������ָ��
}BiTNode, *BitTree;
Status InitBiTree(BitTree *L);
Status DestroyBiTree(BitTree *L);
Status CreateBiTree(BitTree *L);
Status ClearBiTree(BitTree *L);
Status BiTreeEmpty(BitTree L);
Status BiTreeDepth(BitTree L);
BitTree Root(BitTree L);
char Value(BitTree L, ElemType key);
Status Assign(BitTree *L, ElemType key, char data);
BitTree* Parent(BitTree L, ElemType key, BitTree *node);
BitTree LeftChild(BitTree L, ElemType key, BitTree *node);
BitTree RightChild(BitTree L, ElemType key, BitTree *node);
BitTree LeftSibling(BitTree L, ElemType key, BitTree *node);
BitTree RightSibling(BitTree L, ElemType key, BitTree *node);
BitTree FindNode(BitTree L, ElemType key, BitTree *node);
Status InsertChild(BitTree *L, BitTree p, int LR, BitTree c);
Status DeleteChild(BitTree *L, BitTree p, int LR);
Status PreOrderTraverse(BitTree L);
Status InOrderTraverse(BitTree L);
Status PostOrderTraverse(BitTree L);
Status LevelOrderTraverse(BitTree L);
Status SaveBiTree(FILE *fp,BitTree L);
Status ReadBiTree(FILE *fp,BitTree *L);
BitTree Search(BitTree L, ElemType key);
char* Filename(void);
FILE *fp;
char *filename, *pc, appe[5]=".dat",input[60]; //�����������ļ���
int main(void)
{
	BitTree L=NULL,*p,c=NULL;
	FILE *fp;
	char ch,*filename,data;
	int LR,n,op = 1;
	ElemType key;
	p = (BitTree *)malloc(sizeof(BitTree));
	InitBiTree(&c);
	while (op) {
		*p = NULL;
		system("cls");	printf("\n\n");
		printf("      Menu for Binary Tree On Node Structure \n");
		printf("-------------------------------------------------\n");
		printf("    	  1. InitBiTree          12.RightChild\n");
		printf("    	  2. DestroyBiTree       13.LeftSibling\n");
		printf("    	  3. CreateBiTree        14.RightSibling \n");
		printf("    	  4. ClearBiTree         15.InsertChild\n");
		printf("    	  5. BiTreeEmpty         16.DeleteChild\n");
		printf("    	  6. BiTreeDepth         17.PreOrderTraverse\n");
		printf("          7. Root                18.InOrderTraverse\n");
		printf("          8. Value               19.PostOrderTraverse\n");
		printf("          9. Assign              20.LevelOrderTraverse\n");
		printf("          10. Parent             21.SaveBiTree\n");
		printf("          11. LeftChild          22.ReadBiTree\n");
		printf("          0. Exit\n");
		printf("\n");
		printf("-------------------------------------------------\n");
		printf("    ��ѡ����Ĳ���[0~22] ");
		scanf("%d", &op);
		switch (op) {
		case 1:
			if (InitBiTree(&L) == OK)		printf("��ʼ���������ɹ�");
			else				printf("��ʼ��������ʧ��");
			getchar(); getchar();			break;
		case 2:
			if (!L)             printf("������δ��ʼ��");
			else if (DestroyBiTree(&L) == OK)
			{
				printf("���������ٳɹ�");		L = NULL;
			}
			else		printf("����������ʧ��");
			getchar(); getchar();			break;
		case 3:
			if (!L)                             printf("������δ��ʼ��");
			else if (L->key != -1)              printf("�������Ѵ���");   //ֱ�����ж����յ���������Ϊ�ɿ�
			else if (CreateBiTree(&L) == OK)	printf("�����ɹ�");
			else                                printf("����ʧ��");
			getchar();getchar();			break;
		case 4:
			if (!L)                             printf("������δ��ʼ��");
			else if (L->key==-1)                printf("������Ϊ��");
			else if (ClearBiTree(&L) == OK)     printf("��ճɹ�");
			else				                printf("���ʧ��");
			getchar();getchar();			break;
		case 5:
			if (!L)                             printf("������δ��ʼ��");
			else if (BiTreeEmpty(L) ==TRUE)     printf("������Ϊ��");
			else                                printf("�������ǿ�");
			getchar();getchar();			break;
		case 6:
			if (!L)                 printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");    //���յ�������
			else        printf("���������Ϊ%d", BiTreeDepth(L));
			getchar();getchar();			break;
		case 7:
			if (!L)		            printf("������δ��ʼ��");
            else if (L->key==-1)    printf("������Ϊ��");
			else if (Root(L))       printf("���ڵ�keyΪ%d", L->key);
			getchar();getchar();			break;
		case 8:
			if (!L)                 printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("������Ŀ����key\n");
                scanf("%d", &key);			getchar();
                data=Value(L, key); //��ֵ���ʽ��Ϊ�ж������ڵ�ǰ���뻷���±�warning,��˾������Ϊ�������
                if (data)     printf("key��dataΪ%c", data);
                else          printf("���ҽڵ�ʧ��"); //ʧ��ԭ��֮������ں���������
			}
			getchar();getchar();			break;
		case 9:
			if (!L)                 printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("������Ŀ��ڵ�key\n");
                scanf("%d", &key);			getchar();
                printf("��������data\n");
                scanf("%c", &data);			getchar();
                if (Assign(&L, key, data) == OK)    printf("��ֵ�ɹ�");
                else	printf("��ֵʧ��");
			}
			getchar();getchar();			break;
		case 10:
			if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
            else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("��λ���key:");
                scanf("%d", &key);			getchar();
                if(!Search(L,key))  printf("δ�ҵ�Ŀ��ڵ�");
                else if (Parent(L, key ,p)!=NULL)
                    printf("˫��keyΪ%d", (*p)->key);
                else	printf("����ʧ��");
            }
			getchar();getchar();			break;
		case 11:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
            else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("��λ���key:");
                scanf("%d", &key);			getchar();
                if(!Search(L,key))  printf("δ�ҵ�Ŀ��ڵ�");
                else if (LeftChild(L, key, p)!=NULL)
                    printf("����keyΪ%d",(*p)->key);
                else    printf("�ڵ������� ����ʧ��");
            }
			getchar();getchar();			break;
		case 12:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("��λ���key:");
                scanf("%d", &key);          getchar();
                if(!Search(L,key))  printf("δ�ҵ�Ŀ��ڵ�");
                else if (RightChild(L, key, p))
                    printf("�Һ���keyΪ%d",(*p)->key);
                else    printf("�ڵ����Һ��� ����ʧ��");
			}
			getchar();getchar();			break;
		case 13:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("��λ���key:");
                scanf("%d", &key);          getchar();
                if(!Search(L,key))  printf("δ�ҵ�Ŀ��ڵ�");
                else if (LeftSibling(L, key, p))
                    printf("���ֵ�keyΪ%d",(*p)->key);
                else    printf("�ڵ������ֵ� ����ʧ��");
			}
			getchar();getchar();			break;
		case 14:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{   //Ҫ���еݹ�ĺ��������ڽ����뼰�����ж�����д�룬�ʴ˲�����֮ǰ��֮����Ƴ�����
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("��λ���key:");
                scanf("%d", &key);         getchar();
                if(!Search(L,key))  printf("δ�ҵ�Ŀ��ڵ�");
                else if (RightSibling(L, key, p))
                    printf("���ֵ�keyΪ%d",(*p)->key);
                else
                    printf("�ڵ������ֵ� ����ʧ��");
			}
			getchar();getchar();			break;
		case 15:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                ClearBiTree(&(c));
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("����������,����CreateBiTree\n");
                CreateBiTree(&(c));
                printf("����λ��p��key:");
                scanf("%d", &key);
                FindNode(L, key, p);    //Ѱ����key��Ӧ�Ľ�㣬������ָ��
                while ((ch = getchar()) != EOF && ch != '\n');
                if (*p == NULL) 	printf("δ�ҵ�Ŀ��ڵ�");
                else
                {
                    printf("����LR��ֵ��");
                    scanf("%d", &LR);   getchar();
                    if (InsertChild(&L, *p, LR, c) == OK)
                        printf("����ɹ�");
                    else    printf("����ʧ��");
                }
			}
			getchar();getchar();			break;
		case 16:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                while ((ch = getchar()) != EOF && ch != '\n');
                printf("ɾ��λ��p��key:");
                scanf("%d", &key);			getchar();
                FindNode(L, key, p);
                if (*p == NULL)     printf("δ�ҵ�Ŀ��ڵ�");
                else
                {
                    printf("������LR��ֵ��");
                    scanf("%d", &LR);   getchar();
                    n = DeleteChild(&L, *p, LR);
                    if (n == OK)            printf("ɾ���ɹ�");
                    else if (n == ERROR)    printf("ɾ��ʧ��");
                    else                    printf("����ɾ��������");
                }
			}
			getchar();getchar();			break;
		case 17:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
			printf("\n----------------PreOrderTraverse all elements------------------\n");
			PreOrderTraverse(L);
			printf("\n----------------------------end---------------------------------\n");
			}
			getchar();getchar();			break;
		case 18:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
            else{
                printf("\n----------------InOrderTraverse all elements------------------\n");
                InOrderTraverse(L);
                printf("\n----------------------------end---------------------------------\n");
            }
			getchar();getchar();			break;
		case 19:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                printf("\n----------------PostOrderTraverse all elements------------------\n");
                PostOrderTraverse(L);
                printf("\n----------------------------end---------------------------------\n");
            }
			getchar();getchar();			break;
		case 20:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                printf("\n----------------LevelOrderTraverse all elements------------------\n");
                LevelOrderTraverse(L);
                printf("\n----------------------------end---------------------------------\n");
            }
			getchar();getchar();			break;
		case 21:
            if (!L)				    printf("������δ��ʼ��");
			else if (L->key==-1)    printf("������Ϊ��");
			else{
                filename = Filename();  //��ϵͳʱ���Զ������ļ���
                if ((fp = fopen(filename, "w")) == NULL){
                    printf("�ļ���ʧ�� ");  return ERROR;
                }
                else if (SaveBiTree(fp,L) == OK){
                    fclose(fp);				printf("�����ļ��ɹ�");
                }
                else    printf("�����ļ�ʧ��");
			}
			getchar();getchar();			break;
		case 22:
            if (!L)				printf("������δ��ʼ��");
			else{
                if (BiTreeEmpty(L) != TRUE)
                    ClearBiTree(&L);
                free(L);
                L=NULL;
                printf("������Ҫ��ȡ���ļ�����");
                while ((ch = getchar()) != EOF && ch != '\n');
                scanf("%s", input);  getchar();
                filename=input;     //��input����buffer��filename�������
                if ((fp = fopen(filename, "r")) == NULL)
                    printf("�ļ���ʧ��");
                else if (ReadBiTree(fp, &L) == OK)
                {
                    fclose(fp); printf("�ļ���ȡ�ɹ�");
                }
                else
                    printf("�ļ���ȡʧ��");
			}
			getchar();getchar();			break;
		case 0:
			break;
		}
	}
	printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
	getchar();getchar();
	return 0;
}
/**
 * �������ƣ�InitBiTree
 * ����������������ͷָ��L�ĵ�ַ
 * �������ܣ�����һ��20���յĶ�������ͷ
 * ����ֵ���ɹ����췵��OK�����򷵻�ERROR
 **/
Status InitBiTree(BitTree *L){
	if (*L)		return ERROR;
    *L = (BitTree)malloc(sizeof(BiTNode));
	if (*L)
	{
		(*L)->key = -1;     //��ʼ��Ϊ����
		(*L)->lchild = NULL;		(*L)->rchild = NULL;    //����ָ���ÿ�
		return OK;
	}
    return ERROR;
}
/**
 * �������ƣ�DestroyBiTree
 * ����������������ͷָ��L�ĵ�ַ
 * �������ܣ�ɾ��������
 * ����ֵ���ɹ����ٷ���OK�����򷵻�ERROR
 **/
Status DestroyBiTree(BitTree *L){
	if ((*L)==NULL)		return ERROR;
	if ((*L)->lchild != NULL)
        DestroyBiTree(&(*L)->lchild);
	if ((*L)->rchild != NULL)
        DestroyBiTree(&(*L)->rchild);
	free(*L);   //�ݹ��ͷŽڵ�
	return OK;
}
/**
 * �������ƣ�CreateBiTree
 * ����������������ͷָ��L�ĵ�ַ
 * �������ܣ�����������
 * ����ֵ���ɹ���������OK�����򷵻�ERROR
 **/
Status CreateBiTree(BitTree *L){
	int keyin;
	if(!(*L))   return ERROR;
    printf("��������ؼ��֣�-1��ʾ������\n");   //-1��ΪNULL�ı�ǣ�ͬ��Ҳ��һ�εݹ�������������еĿ�ʼ
	scanf("%d", &keyin);	getchar();
	if (keyin==-1)
	{		(*L) = NULL;		return 0;	}
	else
	{
		if (!((*L = (BiTNode *)malloc(sizeof(BiTNode))))){
            printf("�ռ����ʧ��");   return ERROR;
		}
		(*L)->key = keyin;
        printf("��������ֵ��\n");
		scanf("%c", &((*L)->data));		getchar();
		CreateBiTree(&((*L)->lchild));
		CreateBiTree(&((*L)->rchild));  //�ݹ齨���ڵ�
	}
	return OK;
}
/**
 * �������ƣ�ClearBiTree
 * ����������������ͷָ��L�ĵ�ַ
 * �������ܣ����ö�����
 * ����ֵ���ɹ��ÿշ���OK�����򷵻�ERROR
 **/
Status ClearBiTree(BitTree *L){
	if (!DestroyBiTree(&(*L)))		return ERROR;
	*L = NULL;
	if (!InitBiTree(&(*L))) 		return ERROR;
	return OK;
}
/**
 * �������ƣ�BiTreeEmpty
 * ����������������ͷָ��L
 * �������ܣ��ж϶������Ƿ�Ϊ��
 * ����ֵ����TRUE,�ǿ�FALSE
 **/
Status BiTreeEmpty(BitTree L){
	if (L->key==-1)		return TRUE;
	else        		return FALSE;
}
/**
 * �������ƣ�BiTreeDepth
 * ����������������ͷָ��L
 * �������ܣ�����������
 * ����ֵ�����depth
 **/
int BiTreeDepth(BitTree L){
	int depth = 0,lchilddepth,rchilddepth;
	if (L)
	{
		lchilddepth=BiTreeDepth((L->lchild));
		rchilddepth=BiTreeDepth((L->rchild));
		depth = rchilddepth >= lchilddepth ? rchilddepth + 1 : lchilddepth + 1;
	}
	return depth;
}
/**
 * �������ƣ�Root
 * ����������������ͷָ��L
 * �������ܣ��Ҷ������ĸ��ڵ�
 * ����ֵ�����ڵ�L
 **/
BitTree Root(BitTree L){
	return L;
}
/**
 * �������ƣ�Value
 * ����������������ͷָ��L���ؼ���key
 * �������ܣ��Ҷ������йؼ���Ϊkey�Ľڵ��data
 * ����ֵ���ɹ�����data��ʧ�ܷ���0
 **/
char Value(BitTree L, ElemType key){
    BitTree M = Search(L, key);
    if(M)   return M->data;
    printf("δ�ҵ��ýڵ� ");  return 0;
}
/**
 * �������ƣ�Assign
 * ����������������ͷָ��L�ĵ�ַ���ؼ���key�� ������data
 * �������ܣ����������йؼ���Ϊkey�Ľڵ��data��ֵΪvalue
 * ����ֵ���ɹ���ֵ����OK�����򷵻�ERROR
 **/
Status Assign(BitTree *L, ElemType key, char data){
	if (*L==NULL)		return 0;
	BitTree M = Search(*L, key);
	if(M){    M->data=data;   return OK;      }
	printf("δ�ҵ��ýڵ� ");  return 0;
}
/**
 * �������ƣ�Parent
 * ����������������ͷָ��L���ؼ���key
 * �������ܣ��Ҷ������йؼ���Ϊkey�Ľڵ��˫��
 * ����ֵ���ɹ�����˫�׽ڵ�ָ�룬���򷵻�NULL
 **/
BitTree* Parent(BitTree L, ElemType key,BitTree *node)
{
	if(L==NULL)     return NULL;
	if (L->key == key){
        printf("Ŀ��ڵ�Ϊͷ��� ��˫�� ");    return NULL;
	}
	if ((L->lchild) != NULL)
		if (L->lchild->key == key)
		{	*node = L;		return node;	}
	if ((L->rchild)!=NULL)
		if (L->rchild->key == key)
		{    *node = L;     return node;	}
	if (Parent(L->lchild, key ,node))		return node;
	if (Parent(L->rchild, key ,node))		return node;
    return NULL;
}
/**
 * �������ƣ�LeftChild
 * ����������������ͷָ��L���ؼ���key
 * �������ܣ��Ҷ������йؼ���Ϊkey�Ľڵ������
 * ����ֵ���ɹ��������ӽڵ�ָ�룬���򷵻�NULL
 **/
BitTree LeftChild(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->key == key)
		if (L->lchild != NULL)
		{   *node = L->lchild;  return *node;   }
	if (LeftChild(L->lchild, key, node))		return *node;;
	if (LeftChild(L->rchild, key, node))		return *node;
	return NULL;
}
/**
 * �������ƣ�RightChild
 * ����������������ͷָ��L���ؼ���key
 * �������ܣ��Ҷ������йؼ���Ϊkey�Ľڵ���Һ���
 * ����ֵ���ɹ������Һ��ӽڵ�ָ�룬���򷵻�NULL
 **/
BitTree RightChild(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->key == key)
		if (L->rchild != NULL)
		{   *node = L->rchild;  return *node;   }
	if (RightChild(L->lchild, key, node))		return *node;
	if (RightChild(L->rchild, key, node))		return *node;
	return NULL;
}
/**
 * �������ƣ�LeftSibling
 * ����������������ͷָ��L���ؼ���key
 * �������ܣ��Ҷ������йؼ���Ϊkey�Ľڵ�����ֵ�
 * ����ֵ���ɹ��������ֵܽڵ�ָ�룬���򷵻�NULL
 **/
BitTree LeftSibling(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->rchild != NULL)
		if (L->rchild->key == key && L->lchild != NULL)
		{	*node = L->lchild;  return *node;   }
	if (LeftSibling(L->lchild, key,node))		return *node;
	if (LeftSibling(L->rchild, key, node))		return *node;
	return NULL;
}
/**
 * �������ƣ�RightSibling
 * ����������������ͷָ��L���ؼ���key
 * �������ܣ��Ҷ������йؼ���Ϊkey�Ľڵ�����ֵ�
 * ����ֵ���ɹ��������ֵܽڵ�ָ�룬���򷵻�NULL
 **/
BitTree RightSibling(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->lchild != NULL)
		if (L->lchild->key == key && L->rchild != NULL)
		{   *node = L->rchild;      return *node;   }
	if (RightSibling(L->lchild, key, node))		return *node;
	if (RightSibling(L->rchild, key, node))		return *node;
	return NULL;
}
/**
 * �������ƣ�InsertChild
 * ����������������ͷָ��L�ĵ�ַ�����������ָ��p, ��λ����LR�����������ָ��c
 * �������ܣ��ڽڵ�p����������c
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status InsertChild(BitTree *L,BitTree p, int LR, BitTree c)
{
	if (LR == 0){
        if (p->rchild != NULL){
            printf("������C����������\n");  return ERROR;
        }
	    p->lchild = c;  return OK;
    }
	else if (LR == 1){
        if (p->rchild != NULL){
            printf("������C����������\n");  return ERROR;
        }
        p->rchild = c;  return OK;
    }
	else    return ERROR;
}
/**
 * �������ƣ�DeleteChild
 * ����������������ͷָ��L�ĵ�ַ�����������ָ��p, ��λ����LR
 * �������ܣ��ڽڵ�p��ɾ����/������c
 * ����ֵ���ɹ�����OK�����򷵻�ERROR
 **/
Status DeleteChild(BitTree *L, BitTree p, int LR)
{
	if (LR == 0)
	{
		if ((*L)->lchild == NULL)	return FALSE;
		DestroyBiTree(&((*L)->lchild));
		p->lchild = NULL;
	}
	else if (LR == 1)
	{
		if ((*L)->rchild == NULL)   return FALSE;
		DestroyBiTree(&((*L)->rchild));
		p->rchild = NULL;
	}
	else		return ERROR;
	return OK;
}
/**
 * �������ƣ�PreOrderTraverse
 * ����������������ͷָ��L
 * �������ܣ��������������
 * ����ֵ��OK
 **/
Status PreOrderTraverse(BitTree L)
{
	if (L == NULL)
		return 0;
	printf("%d-%c  ", L->key,L->data);
	if (L->lchild != NULL)
		PreOrderTraverse(L->lchild);
	if (L->rchild != NULL)
		PreOrderTraverse(L->rchild);
	return OK;
}
/**
 * �������ƣ�InOrderTraverse
 * ����������������ͷָ��L
 * �������ܣ��������������
 * ����ֵ��OK
 **/
Status InOrderTraverse(BitTree L)
{
	if (L == NULL)
		return 0;
	if (L->lchild != NULL)
		InOrderTraverse(L->lchild);
	printf("%d-%c  ", L->key,L->data);
	if (L->rchild != NULL)
		InOrderTraverse(L->rchild);
	return OK;
}
/**
 * �������ƣ�PostOrderTraverse
 * ����������������ͷָ��L
 * �������ܣ��������������
 * ����ֵ��OK
 **/
Status PostOrderTraverse(BitTree L)
{
	if (L == NULL)
		return 0;
	if (L->lchild != NULL)
		PostOrderTraverse(L->lchild);
	if (L->rchild != NULL)
		PostOrderTraverse(L->rchild);
	printf("%d-%c  ", L->key,L->data);
	return OK;
}
/**
 * �������ƣ�LevelOrderTraverse
 * ����������������ͷָ��L
 * �������ܣ��������������
 * ����ֵ��OK
 **/
Status LevelOrderTraverse(BitTree T)
{
	if (T == NULL)		return 0;
	BitTree q[1000];
	q[0] = T;
	int front = 0,rear = 1;
	while (front < rear)
	{
		printf("%d-%c  ", q[front]->key,q[front]->data);
		if (q[front]->lchild)			q[rear++] = q[front]->lchild;
		if (q[front]->rchild)			q[rear++] = q[front]->rchild;
		++front;
	}
	return OK;
}
/**
 * �������ƣ�SaveBiTree
 * �����������ļ�ָ��fp��������ͷָ��L
 * �������ܣ���L�е����ݱ��浽fpָ��ָ����ļ���
 * ����ֵ��OK
 **/
Status SaveBiTree(FILE *fp, BitTree L)
{
    ElemType c= -1;
	if (L == NULL)
		fwrite(&(c),sizeof(ElemType),1,fp);
	else
	{
		fwrite(&(L->key),sizeof(ElemType),1,fp);
		fwrite(&(L->data),sizeof(char),1,fp);
		SaveBiTree(fp, L->lchild);
		SaveBiTree(fp, L->rchild);
	}
	return OK;
}
/**
 * �������ƣ�LoadBiTree
 * �����������ļ�ָ��fp��������ͷָ��L
 * �������ܣ����ݼ��أ�Ϊ����������Ԫ�ء�
 * ����ֵ��OK
 **/
Status ReadBiTree(FILE *fp, BitTree *L)
{
	ElemType key;
	fread(&(key),sizeof(ElemType),1,fp);
	if (key == -1)		*L = NULL;
	else
	{
		*L = (BitTree)malloc(sizeof(BiTNode));
		(*L)->key = key;
		fread(&((*L)->data),sizeof(char),1,fp);
		ReadBiTree(fp, &(*L)->lchild);
		ReadBiTree(fp, &(*L)->rchild);
	}
	return OK;
}
/**
 * �������ƣ�FindNode
 * ����������������ͷָ��L���ؼ���key�����������ָ��node�ĵ�ַ,
 * �������ܣ�Ѱ�ҹؼ���Ϊkey�Ľڵ㲢������ָ��
 * ����ֵ���ɹ�����node�ĵ�ַ�����򷵻�NULL
 **/
BitTree FindNode(BitTree L, ElemType key, BitTree *node)
{
	if (L == NULL)		return NULL;
	if (L->key == key){   *node = L;  return *node;   }
	if (L->lchild != NULL&& FindNode(L->lchild, key, node))
        return *node;
	if (L->rchild != NULL&& FindNode(L->rchild, key, node))
		return *node;
	return NULL;
}
/**
 * �������ƣ�Search
 * ����������������ͷָ��L���ؼ���key
 * �������ܣ�Ѱ�ҹؼ���Ϊkey�Ľڵ㲢������ָ��
 * ����ֵ���ɹ�����M�ĵ�ַ�����򷵻�NULL
 **/
BitTree Search(BitTree L, ElemType key){
    BitTree M=NULL;
    if(L==NULL)     return M;
    if(L->key==key)   M=L;
    if(!M)  M=Search(L->lchild,key);
    if(!M)  M=Search(L->rchild,key);
    return M;
}
/**
 * �������ƣ�Filename
 * ������������
 * �������ܣ�����ϵͳʱ���Զ������ļ���������
 * ����ֵ������ʱ���ַ���ָ��
 **/
char* Filename(void){
    time_t lt;
    lt=time(NULL);
    filename=ctime(&lt);
    filename[strlen(filename)-1]='\0';
    strcat(filename,appe);      //��������ʱ���������ļ����ַ���
    pc=&filename[0];
    while(*pc){       //�ļ����ַ����淶��������:����' ' �á�_������
        if((*pc==':' ) ||   (*pc==' ')){   *pc='_';    }
        pc++;
    }
    return filename;
}
