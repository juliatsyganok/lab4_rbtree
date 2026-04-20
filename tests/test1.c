#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "rbtree.h"


void test_init() {
    RBTree* tree = rbt_create();
    assert(tree != NULL);
    assert(tree->root == tree->NIL); 
    assert(tree->NIL->color == BLACK); 
    rbt_free(tree);
    printf("Тест 1 создание дерева\n");
}

void test_insert_one() {
    RBTree* tree = rbt_create();
    rbt_insert(tree, "key1", 10.5);
    
    assert(tree->root != tree->NIL);
    assert(strcmp(tree->root->key, "key1") == 0);
    assert(tree->root->value == 10.5);
    assert(tree->root->color == BLACK); 
    
    rbt_free(tree);
    printf("Тест 2 Вставка одного элемента\n");
}

void test_search() {
    RBTree* tree = rbt_create();
    rbt_insert(tree, "abc", 1.0);
    rbt_insert(tree, "def", 2.0);
    RBNode* found = rbt_search(tree, "abc");
    assert(found != NULL && found != tree->NIL);
    assert(found->value == 1.0);
    RBNode* not_found = rbt_search(tree, "xyz");
    assert(not_found == NULL || not_found == tree->NIL);

    rbt_free(tree);
    printf("Тест 3 Поиск\n");
}

void test_balance() {
    RBTree* tree = rbt_create();
    rbt_insert(tree, "1", 10);
    rbt_insert(tree, "2", 20);
    rbt_insert(tree, "3", 30);

    assert(strcmp(tree->root->key, "2") == 0);
    assert(tree->root->color == BLACK);
    assert(tree->root->left->color == RED);
    assert(tree->root->right->color == RED);

    rbt_free(tree);
    printf("Тест 4 Баланировка поворотом\n");
}

void test_delete() {
    RBTree* tree = rbt_create();
    rbt_insert(tree, "del", 100);
    assert(rbt_search(tree, "del") != tree->NIL);

    rbt_delete(tree, "del");
    assert(rbt_search(tree, "del") == tree->NIL);
    assert(tree->root == tree->NIL); 

    rbt_free(tree);
    printf("Тест 5 Удаление\n");
}