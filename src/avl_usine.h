#ifndef AVL_USINE_H
#define AVL_USINE_H
#include "usine.h"

typedef struct AvlUsine {
    Usine* usine;              // Donnée stockée
    int hauteur;               // Hauteur du noeud
    struct AvlUsine* gauche;   // Fils gauche
    struct AvlUsine* droite;   // Fils droit
} AvlUsine;

AvlUsine* inserer_usine(AvlUsine* racine, Usine* u);

Usine* chercher_usine(AvlUsine* racine, const char* id);

void parcours_inverse(AvlUsine* racine, FILE* out, int mode);

void liberer_avl_usine(AvlUsine* racine);

#endif

