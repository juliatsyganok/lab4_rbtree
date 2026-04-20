#include "rbtree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void fix_insert(RBTree* tree, RBNode* z);
static void fix_delete(RBTree* tree, RBNode* x);

RBTree* rbt_create() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    if (!tree) return NULL;
    tree->NIL = (RBNode*)malloc(sizeof(RBNode));
    if (!tree->NIL) {
        free(tree);
        return NULL;
    }
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL;
    tree->NIL->right = tree->NIL;
    tree->NIL->parent = tree->NIL;
    tree->root = tree->NIL;

    return tree;
}


RBNode* create_node(RBTree* tree, const char* key, double value) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    if (!node) return tree->NIL;

    strncpy(node->key, key, 6);
    node->key[6] = '\0';
    node->value = value;
    node->color = RED;
    node->left = tree->NIL;
    node->right = tree->NIL;
    node->parent = tree->NIL;

    return node;
}

void free_node(RBTree* tree, RBNode* node) {
    if (node == tree->NIL) return;

    free_node(tree, node->left);
    free_node(tree, node->right);
    free(node);
}

void rbt_free(RBTree* tree) {
    if (!tree) return;

    free_node(tree, tree->root);
    free(tree->NIL);
    free(tree);
}

static void rotate_left(RBTree* tree, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != tree->NIL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->NIL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void rotate_right(RBTree* tree, RBNode* x) {
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right != tree->NIL)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->NIL)
        tree->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void rbt_insert(RBTree* tree, const char* key, double value) {
    RBNode* z = create_node(tree, key, value);
    RBNode* y = tree->NIL;
    RBNode* x = tree->root;
    while (x != tree->NIL) {
        y = x;
        int cmp = strcmp(key, x->key);
        if (cmp < 0)
            x = x->left;
        else if (cmp > 0)
            x = x->right;
        else {
            x->value = value;
            free(z);
            return;
        }
    }
    z->parent = y;
    if (y == tree->NIL)
        tree->root = z;
    else if (strcmp(key, y->key) < 0)
        y->left = z;
    else
        y->right = z;
    fix_insert(tree, z);
}

static void fix_insert(RBTree* tree, RBNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(tree, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

RBNode* rbt_search(RBTree* tree, const char* key) {
    RBNode* current = tree->root;
    while (current != tree->NIL) {
        int cmp = strcmp(key, current->key);
        if (cmp < 0)
            current = current->left;
        else if (cmp > 0)
            current = current->right;
        else
            return current;
    }
    return tree->NIL;
}

static RBNode* tree_minimum(RBTree* tree, RBNode* node) {
    while (node->left != tree->NIL)
        node = node->left;
    return node;
}

static void transplant(RBTree* tree, RBNode* u, RBNode* v) {
    if (u->parent == tree->NIL)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void rbt_delete(RBTree* tree, const char* key) {
    RBNode* z = rbt_search(tree, key);
    if (!z) return;
    RBNode* y = z;
    RBNode* x;
    int y_original_color = y->color;

    if (z->left == tree->NIL) {
        x = z->right;
        transplant(tree, z, z->right);
    }
    else if (z->right == tree->NIL) {
        x = z->left;
        transplant(tree, z, z->left);
    }
    else {
        y = tree_minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    if (y_original_color == BLACK)
        fix_delete(tree, x);
}

static void fix_delete(RBTree* tree, RBNode* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

void rbt_print(RBTree* tree, RBNode* node, int level) {
    if (node == tree->NIL) {
        return;
    }
    rbt_print(tree, node->right, level + 1);

    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    char color_char = (node->color == RED) ? 'R' : 'B';
    printf("[%s: %.2f %c]\n", node->key, node->value, color_char);
    rbt_print(tree, node->left, level + 1);
}