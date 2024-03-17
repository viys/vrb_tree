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
    node->color = color;
}

static rb_color vrbtree_get_color(rbtree_node* node)
{
    return node->color;
}

static rbtree_node* vrbtree_get_parent(rbtree_node* node)
{
    vassert(node != NULL, "");
    return node->parent;
}

static int is_root(rbtree_node* node)
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

    ret = this->compare(node->key,child->key);

    vassert(ret != 0, "");

    if(ret > 0){
        node->left = child;
    }else{
        node->right = child;
    }
}

static void vrbtree_rotate_left(void* t, rbtree_node* node)
{
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
    this->set_parent(parent,q);
    this->set_parent(q,p);

    p->right = q->left;
    if(q->left)
        this->set_parent(p,q->left);
    q->left = p;
}

static void vrbtree_rotate_right(void* t, rbtree_node *node)
{
    RBTREE* this = (RBTREE*)t;
    rbtree_node *p = node;
    rbtree_node *q = node->left; /* can't be NULL */
    rbtree_node *parent = this->get_parent(p);

    if (!is_root(p)) {
        if (parent->left == p)
            parent->left = q;
        else
            parent->right = q;
    } else
        this->root = q;
    this->set_parent(parent, q);
    this->set_parent(q, p);

    p->left = q->right;
    if (p->left)
        this->set_parent(p, p->left);
    q->right = p;
}

rbtree_node* vrbtree_lookup(void* t, void* key, rbtree_node** parent)
{
    RBTREE* this = (RBTREE*)t;
    int ret = 0;
    rbtree_node *current = this->root;

    while(current){
        ret = this->compare(key, current->key);
        // vassert(1, "%s\r\n", current->key);
        if(ret == 0){
            return current;
        }else{
            if(parent != NULL){
                *parent = current;
            }

            if (ret > 0){
                current = current->right;
            }else{
                current = current->left;
            }
        }
    };

    return NULL;
}

void vrbtree_init(void* t, rbtree_compare_fn fn)
{
    RBTREE* this = (RBTREE*)t;
    vassert(this!=NULL, "RBTREE init success\r\n");

    this->compare = fn;
    this->root = NULL;
}

rbtree_node* vrbtree_insert(void* t, void *key, void* data)
{
    RBTREE* this = (RBTREE*)t;
    rbtree_node* node = this->creat_node(key,data);
    rbtree_node* samenode = NULL;
    rbtree_node* parent = NULL;

    samenode = this->api.lookup(this, key, &parent);
    if(samenode != NULL){
        vassert(samenode == NULL, "");
    }
    
    node->left = node->right = NULL;
    this->set_color(node, RED);
    this->set_parent(node, parent);

    if(parent == NULL){
        this->root = node;
    }else{
        this->set_child(this, parent, node);
    }

    while((parent = this->get_parent(node)) != NULL && parent->color == RED){
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
    return NULL;
}

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
    
    this->api.init = vrbtree_init;
    this->api.insert = vrbtree_insert;
    this->api.lookup = vrbtree_lookup;

    return this;
}