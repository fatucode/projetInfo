#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "reseaau.h"
#define MAX_ID 100
// ------------------------------------
// Hauteur d’un nœud
// ------------------------------------
int height(AVLNode* n) {
    return (n == NULL) ? 0 : n->height;
}

// ------------------------------------
// Maximum de deux valeurs
// ------------------------------------
int max(int a, int b) {
    return (a > b) ? a : b;
}

// ------------------------------------
// Rotation droite
// ------------------------------------
AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // rotation
    x->right = y;
    y->left = T2;

    // Mise à jour des hauteurs
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height  = max(height(x->left), height(x->right)) + 1;

    return x;
}

// ------------------------------------
// Rotation gauche
// ------------------------------------
AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // rotation
    y->left = x;
    x->right = T2;

    // Mise à jour des hauteurs
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height  = max(height(y->left), height(y->right)) + 1;

    return y;
}

// ------------------------------------
// Facteur d’équilibre
// ------------------------------------
int get_balance(AVLNode* n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

// ------------------------------------
// Recherche dans l’AVL
// ------------------------------------
AVLNode* avl_search(AVLNode* root, const char* id) {
    if (!root) return NULL;

    int cmp = strcmp(id, root->id);

    if (cmp == 0) return root;
    else if (cmp < 0) return avl_search(root->left, id);
    else return avl_search(root->right, id);
}

// ------------------------------------
// Insertion dans l’AVL
// ------------------------------------
AVLNode* avl_insert(AVLNode* root, const char* id, Noeud* ptr_noeud) {

    // 1) Insertion classique BST
    if (root == NULL) {
        AVLNode* node = malloc(sizeof(AVLNode));
        strcpy(node->id, id);
        node->ptr_noeud = ptr_noeud;
        node->left = node->right = NULL;
        node->height = 1;
        return node;
    }

    int cmp = strcmp(id, root->id);

    if (cmp < 0)
        root->left = avl_insert(root->left, id, ptr_noeud);
    else if (cmp > 0)
        root->right = avl_insert(root->right, id, ptr_noeud);
    else
        return root; // ID déjà présent → rien à faire

    // 2) Mise à jour de la hauteur
    root->height = 1 + max(height(root->left), height(root->right));

    // 3) Calcul du déséquilibre
    int balance = get_balance(root);

    // ---- Cas 1 : Left Left ----
    if (balance > 1 && strcmp(id, root->left->id) < 0)
        return rotate_right(root);

    // ---- Cas 2 : Right Right ----
    if (balance < -1 && strcmp(id, root->right->id) > 0)
        return rotate_left(root);

    // ---- Cas 3 : Left Right ----
    if (balance > 1 && strcmp(id, root->left->id) > 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    // ---- Cas 4 : Right Left ----
    if (balance < -1 && strcmp(id, root->right->id) < 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}
void free_avl(AVLNode* root) {
    if (!root) return;
    free_avl(root->left);
    free_avl(root->right);
    free(root);
}

