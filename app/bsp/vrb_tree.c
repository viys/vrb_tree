#include "stdio.h"
#include "stdlib.h"
#include "vrb_tree.h"
#include "vassert.h"

void vrb_tree_test(void)
{
    printf("vrb_tree_test\r\n");
}

static void vrbtree_set_color(rbtree_node* node, rb_color color)
{
    vassert(node != NULL, "");
    node->color = color;
}

static rb_color vrbtree_get_color(rbtree_node* node)
{
    vassert(node != NULL, "");
    return node->color;
}

static rbtree_node* vrbtree_get_parent(rbtree_node* node)
{
    vassert(node != NULL, "");
    return node->parent;
}

int is_root(rbtree_node* node)
{
    vassert(node != NULL, "");
    return (vrbtree_get_parent(node)==NULL);
}

static rbtree_node* vrbtree_creat_node(void* key, void* data)
{
    rbtree_node* newnode = malloc(sizeof(rbtree_node));
    if(newnode == NULL){
        return NULL;
    }

    newnode->key = key;
    newnode->data = data;
    newnode->parent = NULL;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

static void vrbtree_set_parent(rbtree_node* node, rbtree_node* parent)
{
    vassert(node != NULL, "");
    node->parent = parent;
}

static void vrbtree_set_child(void* t, rbtree_node* node, rbtree_node* child)
{
    RBTREE* this = (RBTREE*)t;
    int ret = 0;

    vassert(node != NULL, "");
    vassert(child != NULL, "");

    ret = this->compare(child->key, node->key);

    vassert(ret != 0, "");

    if(ret < 0){
        // printf("%s is %s left child\r\n", child->key, node->key);
        node->left = child;
    }else{
        node->right = child;
        // printf("%s is %s right child\r\n", child->key, node->key);
    }
}

static void vrbtree_rotate_left(void* t, rbtree_node* node)
{
    vassert(node != NULL, "");

    RBTREE* this = (RBTREE*)t;
    rbtree_node* p = node;
    rbtree_node* q = node->right;
    rbtree_node* parent = node->parent;


    if(parent == NULL){
        this->root = q;
    }else{
        if(parent->left == p)
            parent->left = q;
        else
            parent->right = q;
    }

    this->set_parent(q, parent);
    this->set_parent(p, q);

    p->right = q->left;
    if(q->left){
        printf("set_parent %d\r\n", 4);
        this->set_parent(q->left, p);
    }
    q->left = p;
}

static void vrbtree_rotate_right(void* t, rbtree_node *node)
{
    vassert(node != NULL, "");

    RBTREE* this = (RBTREE*)t;
    rbtree_node *p = node;
    rbtree_node *q = node->left; /* can't be NULL */
    rbtree_node *parent = this->get_parent(p);

    if (this->get_parent(p)){
        if(parent->left == p){
            parent->left = q;
        }else{
            parent->right = q;
        }
    }else{
        this->root = q;
    }
        
    this->set_parent(q, parent);
    this->set_parent(p, q);

    p->left = q->right;
    if (p->left){
        this->set_parent(p->left, p);
    }
    q->right = p;
}

rbtree_node* vrbtree_find(void* t, void* key)
{
    RBTREE* this = (RBTREE*)t;
    int ret = 0;
    rbtree_node *current = this->root;

    while(current){
        ret = this->compare(key, current->key);
        
        if(ret == 0){
            return current;
        }else{
            if (ret < 0){
                current = current->left;
            }else{
                current = current->right;
            }
        }
    };

    return NULL;
}

static rbtree_node* vrbtree_lookup(void* t, void* key, rbtree_node** parent)
{
    RBTREE* this = (RBTREE*)t;
    int ret = 0;
    rbtree_node *current = this->root;

    while(current){
        ret = this->compare(key, current->key);
        
        if(ret == 0){
            return current;
        }else{
            if(parent != NULL)
            {
                *parent = current;
            }
            if (ret < 0){
                current = current->left;
            }else{
                current = current->right;
            }
        }
    };

    return NULL;
}

void vrbtree_init(void* t, rbtree_compare_fn fn)
{
    RBTREE* this = (RBTREE*)t;
    vassert(this!=NULL, "");

    this->compare = fn;
    this->root = NULL;
}

static int vrbtree_insert(void* t, void *key, void* data)
{
    RBTREE* this = (RBTREE*)t;
    rbtree_node * node = NULL;
    rbtree_node* samenode = NULL;
    rbtree_node* parent = NULL;

    samenode = this->lookup(this, key, &parent);
    
    if(samenode != NULL){
        vassert(samenode == NULL, "");
        return -1;
    }

    node = this->creat_node(key, data);
    node->left = node->right = NULL;
    this->set_color(node, RED);
    this->set_parent(node, parent);

    if(parent == NULL)
        this->root = node;
    else
    {
        this->set_child(this,parent,node);
    }

    while((parent = this->get_parent(node)) != NULL && parent->color == RED)
    {
        rbtree_node* grandpa = this->get_parent(parent);//grandpa must be existed 
        //because root is black ,and parent is red,
        //parent can not be root of tree. and parent is red,so grandpa must be black
        if(parent == grandpa->left)
        {
            rbtree_node* uncle = grandpa->right;
            if(uncle && this->get_color(uncle) == RED)
            {
                this->set_color(grandpa, RED);
                this->set_color(parent, BLACK);
                this->set_color(uncle, BLACK);
                node = grandpa;
            }
            else
            {
                if(node == parent->right )
                {
                    this->rotate_left(this, parent);
                    node = parent;
                    parent = this->get_parent(parent);
                }
                this->set_color(parent, BLACK);
                this->set_color(grandpa, RED);
                this->rotate_right(this, grandpa);
            }

        }
        else
        {
            rbtree_node* uncle = grandpa->left;
            if(uncle && uncle->color == RED)
            {
                this->set_color(grandpa, RED);
                this->set_color(parent, BLACK);
                this->set_color(uncle, BLACK);
                node = grandpa;
            }
            else
            {
                if(node == parent->left)
                {
                    this->rotate_right(this, parent);
                    node = parent;
                    parent = this->get_parent(node);
                }
                this->set_color(parent, BLACK);
                this->set_color(grandpa, RED);
                this->rotate_left(this, grandpa);
            }
        }
    }

    this->set_color(this->root, BLACK);
    return 0;
}

// {
//     RBTREE* this = (RBTREE*)t;
//     rbtree_node* node = this->creat_node(key,data);
//     rbtree_node* samenode = NULL;
//     rbtree_node* parent = NULL;

//     samenode = this->api.lookup(this, key, &parent);
//     if(samenode != NULL){
//         vassert(samenode == NULL, "");
//         return samenode;
//     }
    
//     node->left = node->right = NULL;
//     this->set_color(node, RED);
//     printf("set_parent %d\r\n", 8);
//     this->set_parent(node, parent);

//     if(parent == NULL){
//         this->root = node;
//     }else{
//         this->set_child(this, parent, node);
//     }

//     while((parent = this->get_parent(node)) != NULL && parent->color == RED){
//         rbtree_node* grandpa = this->get_parent(parent);//grandpa must be existed 
//         //because root is black ,and parent is red,
//         //parent can not be root of tree. and parent is red,so grandpa must be black
//         if(parent == grandpa->left)
//         {
//             rbtree_node* uncle = grandpa->right;
//             if(uncle && this->get_color(uncle) == RED)
//             {
//                 this->set_color(grandpa, RED);
//                 this->set_color(parent, BLACK);
//                 this->set_color(uncle, BLACK);
//                 node = grandpa;
//             }
//             else
//             {
//                 if(node == parent->right )
//                 {
//                     printf("rotate_left %d\r\n", 4);
//                     this->rotate_left(this, parent);
//                     node = parent;
//                     parent = this->get_parent(parent);
//                 }
//                 this->set_color(parent, BLACK);
//                 this->set_color(grandpa, RED);
//                 printf("rotate_right %d\r\n", 1);
//                 this->rotate_right(this, grandpa);
//             }

//         }
//         else
//         {
//             rbtree_node* uncle = grandpa->left;
//             if(uncle && uncle->color == RED)
//             {
//                 this->set_color(grandpa, RED);
//                 this->set_color(parent, BLACK);
//                 this->set_color(uncle, BLACK);
//                 node = grandpa;
//             }
//             else
//             {
//                 if(node == parent->left)
//                 {
//                     printf("rotate_right %d\r\n", 2);
//                     this->rotate_right(this, parent);
//                     node = parent;
//                     parent = this->get_parent(node);
//                 }
//                 this->set_color(parent, BLACK);
//                 this->set_color(grandpa, RED);
//                 printf("rotate_left %d\r\n", 5);
//                 this->rotate_left(this, grandpa);
//             }
//         }
//     }

//     this->set_color(this->root, BLACK);
//     return NULL;
// }

RBTREE* RBTREE_CTOR(void)
{
    RBTREE* this = malloc(sizeof(RBTREE));

    this->creat_node = vrbtree_creat_node;
    this->set_color = vrbtree_set_color;
    this->get_color = vrbtree_get_color;
    this->set_parent = vrbtree_set_parent;
    this->get_parent = vrbtree_get_parent;
    this->set_child = vrbtree_set_child;
    this->rotate_right = vrbtree_rotate_right;
    this->rotate_left = vrbtree_rotate_left;
    this->lookup = vrbtree_lookup;
    
    this->api.init = vrbtree_init;
    this->api.insert = vrbtree_insert;
    this->api.find = vrbtree_find;

    return this;
}