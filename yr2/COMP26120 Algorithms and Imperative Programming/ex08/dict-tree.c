#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define __USE_BSD
#include <string.h>

#include "speller.h"
#include "dict.h"

#define HEIGHT(p)    ( (p==NULL)? 0 : p->depth )
#define MAX(a, b)    ( a>b ? a:b )


extern int mode;
typedef struct node *tree_ptr;
struct node{
    Key_Type element; // only data is the key itself
    tree_ptr left, right;
    int depth;
  // add anything else that you need
};

typedef struct node Node;
struct table{
    tree_ptr head; // points to the head of the tree
    int depth;
    unsigned colli,num;
  // add anything else that you need
};

tree_ptr newNode(Key_Type key);
Table insert(Key_Type key,Table T);
void printNode(tree_ptr node);
int depth(tree_ptr node);
tree_ptr insertAVL(tree_ptr tree, Key_Type key, unsigned *num);
tree_ptr left_left_rotation(tree_ptr k2);
tree_ptr right_right_rotation(tree_ptr k1);
tree_ptr left_right_rotation(tree_ptr k3);
tree_ptr right_left_rotation(tree_ptr k1);
void destroyBitree(tree_ptr node);

unsigned word_num=0;

Table initialize_table(/*ignore parameter*/) {
    Table T=(Table)malloc(sizeof(struct table));
    T->head=NULL;
    T->colli=0;
    T->num=0;
    return T;
}

Table insert(Key_Type key,Table T){
    if(T->head==NULL){
        T->head=newNode(key);
        T->num++;
    }
	/* go down BST with its path determined by cmp result of key and node_key*/
	 else{
        if(mode==1){//BST implementation
            tree_ptr p=T->head;
            while(p){
                if(strcmp(p->element,key)==0){
                    //printf("%s Duplicated!\n",key);
                    return T;
                }
                else if(strcmp(p->element,key)>0){
                    if(p->left)     p=p->left;
                    else{
                        //a new node shall be created here as we are at end of the path
                        p->left=newNode(key);
                        T->num++;
                        return T;
                    }
                }
                else {
                    if(p->right)     p=p->right;
                    else{
                        //a new node shall be created here as we are at end of the path
                        p->right=newNode(key);
                        T->num++;
                        return T;
                    }
                }
            }
        }
        else if(mode==2){//AVL implementation
            tree_ptr res=insertAVL(T->head,key,&(T->num));
            if(res){
                T->head=res;
            }
        }
    }
    return T;
}

Boolean find(Key_Type key, Table T){
    word_num++; //word counter
    tree_ptr p=T->head;
    while(p){
        if(!strcmp(p->element, key)){
            //printf("%s found\n",p->element);
            return TRUE;
        }
        if(strcmp(p->element, key)>0){//key is smaller than node_key,go to leftchild
            p=p->left;
            T->colli++;//count avr search depth
        }
        else{//key is larger than node_key,go to rightchild
            p=p->right;
            T->colli++;
        }
    }
    //printf("%s not found\n",key);
    return FALSE;
}

void print_table(Table T){
    printNode(T->head);
    putchar('\n');
}

void print_stats (Table T){
    T->depth = depth(T->head);  //depth of the tree
    printf("node number:%u\ndepth:%d\naverage search depth:%f\n", T->num,T->depth, (T->colli+word_num)*1.0/word_num);
}

void tidy (Table T){
    destroyBitree(T->head);
    free(T);
}

tree_ptr newNode(Key_Type key){/*return a new node with new key*/
    tree_ptr nnode=(tree_ptr)malloc(sizeof(Node));
    nnode->element=(Key_Type)malloc(strlen(key)+1);
    strcpy(nnode->element,key);
    nnode->left=nnode->right=NULL;
    return nnode;
}

void printNode(tree_ptr node){/*preorder traversing*/
    static short i=0;
    printf("%20s ", node->element);
    i++;
    if(i%6==0)  putchar('\n');
    if(node->left){
        printNode(node->left);
    }
    if(node->right){
        printNode(node->right);
    }
}

int depth(tree_ptr node){/*bottom-up calculation*/
    int lchilddepth,rchilddepth;
	if (node)
	{
		lchilddepth=depth((node->left));
		rchilddepth=depth((node->right));
    /*depth of a tree is the bigger one of that of leftchild and rightchild +1*/
		node->depth = rchilddepth >= lchilddepth ? rchilddepth + 1 : lchilddepth + 1;
		return node->depth;
	}
	else{
        return 0;
	}
}

tree_ptr insertAVL(tree_ptr tree, Key_Type key, unsigned *num){
    if (tree == NULL) {
        tree = newNode(key);
        (*num)++;
        if (tree==NULL){
            printf("ERROR: create avltree node failed!\n");
            return NULL;
        }
    }
    else if (strcmp(key,tree->element)<0){ /* go down BST with its path determined by cmp result of key and node_key*/
        tree->left = insertAVL(tree->left, key, num);
        // causing unbalance after insertion on leftchild
        if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2){
            if (strcmp(key,tree->left->element)<0)
                tree = left_left_rotation(tree);  //LL
            else
                tree = left_right_rotation(tree); //LR
        }
    }
    else if (strcmp(key,tree->element)>0){
        tree->right = insertAVL(tree->right, key, num);
        // causing unbalance after insertion on rightchild
        if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2){
            if (strcmp(key,tree->right->element)>0)
                tree = right_right_rotation(tree); //RR
            else
                tree = right_left_rotation(tree);  //RL
        }
    }
    else {
        //printf("Key collision\n");
    }
    tree->depth = MAX( HEIGHT(tree->left), HEIGHT(tree->right))+1;
    return tree;
}

tree_ptr left_left_rotation(tree_ptr k2){
    tree_ptr k1= k2->left;
    k2->left = k1->right;
    k1->right = k2;
    /*updating depth*/
    k2->depth = MAX( HEIGHT(k2->left), HEIGHT(k2->right)) + 1;
    k1->depth = MAX( HEIGHT(k1->left), k2->depth) + 1;
    return k1;
}

tree_ptr right_right_rotation(tree_ptr k1){
    tree_ptr k2= k1->right;
    k1->right = k2->left;
    k2->left = k1;
    /*updating depth*/
    k1->depth = MAX( HEIGHT(k1->left), HEIGHT(k1->right)) + 1;
    k2->depth = MAX( HEIGHT(k2->right), k1->depth) + 1;
    return k2;
}

tree_ptr left_right_rotation(tree_ptr k3){
    k3->left = right_right_rotation(k3->left);
    return left_left_rotation(k3);
}

tree_ptr right_left_rotation(tree_ptr k1){
    k1->right = left_left_rotation(k1->right);
    return right_right_rotation(k1);
}

void destroyBitree(tree_ptr node){//bottom-up cleaning
    if (node==NULL)		return ;
    if(node->left!=NULL){
        destroyBitree(node->left);
        node->left=NULL;
    }
    if(node->right!=NULL){
        destroyBitree(node->right);
        node->right=NULL;
    }
    if (node->element!=NULL){
        free(node->element);
        node->element=NULL;
    }
	free(node);
	node=NULL;
	return;
}
