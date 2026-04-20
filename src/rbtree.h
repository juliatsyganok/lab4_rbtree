#ifndef RBTREE_H
#define RBTREE_H

#define RED   0
#define BLACK 1

typedef struct RBNode {
    char key[7];
    double value;
    int color;

    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
} RBNode;

typedef struct {
    RBNode* root;
    RBNode* NIL; 
} RBTree;

RBTree* rbt_create();
void rbt_free(RBTree* tree);
void rbt_insert(RBTree* tree, const char* key, double value);
void free_node(RBTree* tree, RBNode* node);
RBNode* rbt_minimum(RBTree* tree, RBNode* node);
RBNode* create_node(RBTree* tree, const char* key, double value);
void rbt_delete(RBTree* tree, const char* key);
RBNode* rbt_search(RBTree* tree, const char* key);
void rbt_print(RBTree* tree, RBNode* node, int level);

#endif