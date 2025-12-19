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
int hauteur_usine(AvlUsine* n);
Usine* chercher_usine(AvlUsine* racine, const char* id);
int max(int a, int b);
void parcours_inverse(AvlUsine* racine, FILE* out, int mode);
AvlUsine* creer_noeud_usine(Usine* u);
void liberer_avl_usine(AvlUsine* racine);
int comparer_id(const char* a, const char* b);
int facteur_equilibre_usine(AvlUsine* n);
AvlUsine* rotation_droite_usine(AvlUsine* y);
AvlUsine* rotation_gauche_usine(AvlUsine* x);
AvlUsine* inserer_usine(AvlUsine* racine, Usine* u);
void liberer_avl_usine(AvlUsine* racine);
#endif
