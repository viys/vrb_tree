#include "stdio.h"
#include "string.h"
#include "includes.h"
#include "vrb_tree.h"
#include "vassert.h"
#include "stdlib.h"

RBTREE* Tree = NULL;
RBTREE_IMPLEMENTS* tree = NULL;

void get_input_from_terminal(char** key, char** data) {
    free(*key); // 先释放之前分配的内存（如果有的话）
    free(*data);

    *key = (char*)malloc(50 + 1);
    *data = (char*)malloc(200 + 1);

    if (*key == NULL || *data == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // 从终端读取key
    printf("Enter the key: ");
    fgets(*key, 50 + 1, stdin);
    (*key)[strcspn(*key, "\n")] = '\0'; // 移除末尾的换行符

    // 从终端读取data
    // printf("Enter the data: ");
    // fgets(*data, 200 + 1, stdin);
    // (*data)[strcspn(*data, "\n")] = '\0'; // 移除末尾的换行符
}

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

extern int is_root(rbtree_node* node);


int main(void)
{
    rbtree_node* node = NULL;
    Tree = RBTREE_CTOR();
    tree = (RBTREE_IMPLEMENTS*)Tree;

    vassert(tree!=NULL, "RBTREE_CTOR() success\r\n");

    tree->init(tree, rbtree_compare);

    while (1)
    {
        char* key = NULL;
        char* data = NULL;


        get_input_from_terminal(&key, &data);
        
        tree->insert(tree, key, "1");
        node = tree->find(tree, key);
        if (!is_root(node)) {
            // printf("yezi\r\n");
        }else if(!Tree->get_parent(node)) {
            // printf("root\r\n");            
        }
        print_tree(Tree);
    }

    // tree->insert(tree, "4", "1");
    // node = tree->lookup(tree, "4", NULL);
    // printf("inset %s %s\r\n", node->key, node->data);
}