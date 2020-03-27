#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"


enum { RED, BLACK };

typedef int COLOR;



typedef struct Node {
    date date_in;
	list_node* rec;

	COLOR color;
	struct Node *right, *left, *parent;

}*node;
typedef node* tree;



node initilize(node, date, list_node*);
node grandparent(node);
node uncle(node);
node sibling(node);

node findNode(tree, date);

int insertNode(tree, date, list_node*);
void insertUtil(node);

// int removeNode(tree, char *);
// void removeUtil(node);

void rotateRight(node);
void rotateLeft(node);

//node findNode(tree, date);
void replaceNode(tree, node, node);

void printNode(node n);
void inorderIterator(node n, void(*func)(node));
void inorderPrint(tree t);
void countSick(node n, int* count);
void countPatients(node n, int* count);
void countPatientsRanged(node n, int* count, date indate, date outdate);
void countPatientsRangedCountry(node n, int* count, date indate, date outdate, char* key);

void destroy(node);




