#include "stdio.h"
#include "string.h"
#include "includes.h"
#include "vrb_tree.h"
#include "vassert.h"

RBTREE* Tree = NULL;
RBTREE_IMPLEMENTS* tree = NULL;

#define SIZE 12
typedef unsigned long long ULL;
void padding (int n)
{
    for (int i = 0; i < n; i++ )
        printf ( "\t" );
}

int rbtree_compare(void *key_a,  void *key_b)
{
    char* key1 = (char*)key_a;
    char* key2 = (char*)key_b;

    return strcmp(key1, key2);
}

void print_node(rbtree_node *root, int level)
{

    if ( root == NULL ) 
    {
        padding (level );
        puts ( "NIL" );
    }
    else 
    {
        print_node ( root->right, level + 1 );
        padding (level );
        if(root->color == BLACK)
        {
            printf ( "%s\n", (root->key) );
        }
        else
            printf ( "(%s)\n", (root->key) );
        print_node ( root->left, level + 1 );
    }
}

void print_tree(RBTREE* this)
{
    print_node(this->root,0);
    printf("-------------------------------------------\n");
}

int main(void)
{
    Tree = RBTREE_CTOR();
    tree = (RBTREE_IMPLEMENTS*)Tree;

    vassert(tree!=NULL, "RBTREE_CTOR() success\r\n");

    tree->init(tree, rbtree_compare);
    tree->insert(tree, "2", "1");
    tree->insert(tree, "1", "1");
    tree->insert(tree, "3", "1");
    tree->insert(tree, "4", "1");

    print_tree(Tree);
}