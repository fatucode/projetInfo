#ifndef AVL_H
#define AVL_H

#include "reseaau.h"

/* ----------------------------------------------------
   Structure d’un nœud AVL
---------------------------------------------------- */
typedef struct AVLNode {
    char id[MAX_ID];
    Noeud* ptr_noeud;

    struct AVLNode* gauche;
    struct AVLNode* droite;

    int hauteur;
} AVLNode;

/* ----------------------------------------------------
   Fonctions utilitaires
---------------------------------------------------- */
int hauteur(AVLNode* n);
int max(int a, int b);

/* ----------------------------------------------------
   Recherche / insertion
---------------------------------------------------- */
AVLNode* avl_recherche(AVLNode* racine, const char* id);
AVLNode* inserer_noeud_avl(AVLNode* racine, const char* id, void* ptr_noeud);

/* ----------------------------------------------------
   Rotations AVL
---------------------------------------------------- */
AVLNode* rotation_gauche(AVLNode* x);
AVLNode* rotation_droite(AVLNode* y);
int facteur_equilibre(AVLNode* n);

/* ----------------------------------------------------
   Libération mémoire
---------------------------------------------------- */
void liberer_avl(AVLNode* racine);
#endif
