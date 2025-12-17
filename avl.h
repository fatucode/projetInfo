#ifndef AVL_H
#define AVL_H
#include "reseaau.h"

typedef struct AVLNode {
    char id[MAX_ID];
    Noeud* ptr_noeud;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

// --- Fonctions générales ---
int height(AVLNode* n);
int max(int a, int b);

// --- Recherche / insertion ---
AVLNode* avl_search(AVLNode* root, const char* id);
AVLNode* avl_insert(AVLNode* root, const char* id, Noeud* ptr_noeud);

// --- Rotations ---
AVLNode* rotate_left(AVLNode* x);
AVLNode* rotate_right(AVLNode* y);
int get_balance(AVLNode* n);
void free_avl(AVLNode* root);

#endif
