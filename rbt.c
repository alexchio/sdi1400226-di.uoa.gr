#include "rbt.h"

node initilize(node p, date d, list_node* inrec)
{
	node tree = (node)malloc(sizeof(struct Node));
	tree->left = tree->right = NULL;
	tree->parent = p;
	tree->date_in=d; 	//maybe need to set every data explicitly
	tree->color = RED;
	tree->rec=inrec;
	return tree;
}
node grandparent(node n)	//voithitiko node
{
	if (n == NULL || n->parent == NULL)
		return NULL;
	return n->parent->parent;
}
node uncle(node n)		//voithitiko node
{
	node g = grandparent(n);
	if (n == NULL || g == NULL)
		return NULL;
	if (n->parent == g->left)
		return g->right;
	else
		return g->left;
}
node sibling(node n)		//voithitiko node
{
	if (n == n->parent->left)
		return n->parent->right;
	else
		return n->parent->left;
}
int colorOf(node n)
{
	return n == NULL ? BLACK : n->color;
}

int insertNode(tree t, date d, list_node* inrec)	//eisagwgh new node sto rbt
{
	
	int pl = 0;
	node ptr, btr = NULL, newNode;


	for (ptr = *t; ptr != NULL;		//maybe ==2
	btr = ptr, ptr = ((pl = (date_older(ptr->date_in , d)==2)) ? ptr->left : ptr->right));

	newNode = initilize(btr, d, inrec);

	if (btr != NULL)
		(pl) ? (btr->left = newNode) : (btr->right = newNode);

	insertUtil(newNode);

	ptr = newNode;
	for (ptr = newNode; ptr != NULL; btr = ptr, ptr = ptr->parent);
	*t = btr;
	
	return 1;
}
void insertUtil(node n)		//fix sta nodes tou rbt gia ikanopoihsh twn kanonwn tou meta apo eisagwgh
{
	node u = uncle(n), g = grandparent(n), p = n->parent;
	if (p == NULL)
		n->color = BLACK;
	else if (p->color == BLACK)
		return;
	else if (u != NULL && u->color == RED)
	{
		p->color = BLACK;
		u->color = BLACK;
		g->color = RED;

		insertUtil(g);
	}
	else
	{
		if (n == p->right && p == g->left)
		{
			rotateLeft(p);
			n = n->left;
		}
		else if (n == p->left && p == g->right)
		{
			rotateRight(p);
			n = n->right;
		}

		g = grandparent(n);
		p = n->parent;

		p->color = BLACK;
		g->color = RED;

		if (n == p->left)
			rotateRight(g);
		else
			rotateLeft(g);
	}
}
void replaceNode(tree t, node o, node n)	//voithitiki synarthsh antikatastashs
{
	if (o->parent == NULL)
		*t = n;
	else
	{
		if (o == o->parent->left)
			o->parent->left = n;
		else
			o->parent->right = n;
	}

	if (n != NULL)
		n->parent = o->parent;
}

void rotateRight(node tree)		//deksia peristrofh tou rbt
{
	node c = tree->left;
	node p = tree->parent;

	if (c->right != NULL)
		c->right->parent = tree;

	tree->left = c->right;
	tree->parent = c;
	c->right = tree;
	c->parent = p;

	if (p != NULL)
	{
		if (p->right == tree)
			p->right = c;
		else
			p->left = c;
	}
//	printf("rotation |%s|, right\n", tree->value);
}
void rotateLeft(node tree)	//aristerh peristrofh tou rbt
{
	node c = tree->right;
	node p = tree->parent;

	if (c->left != NULL)
		c->left->parent = tree;

	tree->right = c->left;
	tree->parent = c;
	c->left = tree;
	c->parent = p;

	if (p != NULL)
	{
		if (p->left == tree)
			p->left = c;
		else
			p->right = c;
	}
//	printf("rotation |%s|, left\n", tree->value);
}

node findNode(tree t, date d)		//evresh (kai epistrofh) node apo rbt me sygkekrimeno key
{
	node p=NULL;		//maybe ==2
	for (p = *t; p != NULL && date_older(p->date_in ,d) !=0; p = ((date_older(p->date_in , d)==2) ? p->left : p->right));
	return p;
}
void printNode(node n)		//ektypwsh node
{
//	printf("|%s| (%s)  ", n->value, (n->color == BLACK ? "b" : "r"));
//	printf("|%s| |%s| |%s| |%s| |%s|\n", n->first, n->last, n->age, n->gender, n->taxk);
	printf("%2d/%2d/%4d\n", n->date_in.day, n->date_in.month, n->date_in.year);

}

void countSick(node n, int* count){
	if (n == NULL)
		return;
	countSick(n->left, count);
	if(n->rec->exitDate.year==0)
		(*count)++;	
	countSick(n->right, count);
}

void countPatients(node n, int* count){
	if (n == NULL)
		return;
	countPatients(n->left, count);
	(*count)++;	
	countPatients(n->right, count);
}

void countPatientsRanged(node n, int* count, date indate, date outdate){
	if (n == NULL)
		return;
	countPatientsRanged(n->left, count, indate, outdate);
	if (date_older(n->date_in, indate)==2 && date_older(n->date_in, outdate)==1)
		(*count)++;	
	countPatientsRanged(n->right, count, indate, outdate);
}

void countPatientsRangedCountry(node n, int* count, date indate, date outdate, char* key){
	if (n == NULL)
		return;
	countPatientsRangedCountry(n->left, count, indate, outdate, key);
	if (date_older(n->date_in, indate)==2 && date_older(n->date_in, outdate)==1 && strcmp(n->rec->country, key)==0)
		(*count)++;	
	countPatientsRangedCountry(n->right, count, indate, outdate, key);
}

void inorderInterator(node n, void(*func)(node))	//anadromikh synarthsh gia ylopoihsh opoiasdhpote synarthshs panw sta node (inorder)
{
	if (n == NULL)
		return;
	inorderInterator(n->left, func);
	func(n);
	inorderInterator(n->right, func);
}
void inorderPrint(tree t)		//ektypwsh dentrou me vohthia twn 2 parapanw synarthsewn
{
	inorderInterator(*t, printNode);
	printf("\n");
}

// void inorder_print_file(node t, FILE * fp)
// {
// 	if (t==NULL)
// 		return;
// 	inorder_print_file(t->left, fp);
// 	fprintf(fp,"%s %s %s %s %s %s\n", t->value, t->first,t->last, t->age, t->gender, t->taxk);
// 	inorder_print_file(t->right, fp);

// }
void destroy(node tree)		//apeleftherwsh desmevmenhs mnhmhs gia to dentro
{
	if (tree == NULL)
		return;
	destroy(tree->left);
	destroy(tree->right);
	free(tree);
}

// void check_rbt(tree t, char *v, int flag)	//elegxos gia yparxh node me sygkekrimeno key; flag=1 otan psaxnw mono sto tree, flag=2 otan psaxnw sto tree meta apo to bf
// {
// 	node n=NULL;
// 	n=findNode(t,v);
// 	if (n==NULL)
// 		printf("#KEY %s NOT-IN-RBT\n",v);
// 	else{
// 		if (flag==1)					//search me 'lrb' command
// 			printf("#KEY %s FOUND-IN-RBT\n",v);
// 		if (flag==2)					//search me 'find' command
// 			printf("#REC-IS: %s %s %s %s %s \n", v, n->first, n->last, n->taxk, n->age);
// 	}
// }


