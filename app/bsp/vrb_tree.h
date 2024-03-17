#pragma once

typedef enum 
{
    RED = 0,
    BLACK
}rb_color;

typedef struct _rbtree_node
{
    /*data*/
    void*                   key;
    void*                   data;
    rb_color                color; 
    struct _rbtree_node*    parent; 
    struct _rbtree_node*    left;   
    struct _rbtree_node*    right;  
}rbtree_node;

typedef int (*rbtree_compare_fn)(void *key_a,  void *key_b);

typedef struct
{
    void (*init)(void*, rbtree_compare_fn fn);
    int (*insert)(void*, void *key, void* data);
    rbtree_node* (*find)(void*, void *key);
    int (*remove)(void*, void *key);
}RBTREE_IMPLEMENTS;

typedef struct
{
    RBTREE_IMPLEMENTS api;
    rbtree_node* root;
    rbtree_compare_fn compare;
    void (*set_color)(rbtree_node* node, rb_color color);
    rb_color (*get_color)(rbtree_node* node);
    rbtree_node* (*creat_node)(void* key, void* data);
    void (*set_parent)(rbtree_node* node, rbtree_node* parent);
    rbtree_node* (*get_parent)(rbtree_node* node);
    void (*set_child)(void* t, rbtree_node* node, rbtree_node* child);
    void (*rotate_left)(void*, rbtree_node* node);
    void (*rotate_right)(void*, rbtree_node* node);
    rbtree_node* (*lookup)(void*, void *key, rbtree_node** parent);
    // rbtree_node* (*lock)()
}RBTREE;

RBTREE* RBTREE_CTOR(void);
void RBTREE_DTOR(RBTREE* t);

void vrb_tree_test(void);


