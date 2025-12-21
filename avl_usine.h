#ifndef AVL_USINE_H
#define AVL_USINE_H

#include "usine.h"   // Définition de la structure Usine

// Structure d'un nœud de l'AVL des usines
typedef struct AvlUsine {
    Usine* usine;              // Pointeur vers les données de l'usine
    int hauteur;               // Hauteur du nœud dans l'arbre AVL
    struct AvlUsine* gauche;   // Fils gauche
    struct AvlUsine* droite;   // Fils droit
} AvlUsine;

// Insère une usine dans l'arbre AVL
AvlUsine* inserer_usine(AvlUsine* racine, Usine* u);

// Retourne la hauteur d'un nœud (0 si NULL)
int hauteur_usine(AvlUsine* n);

// Recherche une usine par son identifiant
Usine* chercher_usine(AvlUsine* racine, const char* id);

// Retourne le maximum de deux entiers
int max(int a, int b);

// Parcours inverse de l'arbre (droite → racine → gauche)
// Utilisé pour afficher les usines par ordre décroissant
void parcours_inverse(AvlUsine* racine, FILE* out, int mode);

// Crée un nouveau nœud AVL à partir d'une usine
AvlUsine* creer_noeud_usine(Usine* u);

// Libère toute la mémoire de l'arbre AVL des usines
void liberer_avl_usine(AvlUsine* racine);

// Compare deux identifiants d'usine
int comparer_id(const char* a, const char* b);

// Calcule le facteur d'équilibre d'un nœud
int facteur_equilibre_usine(AvlUsine* n);

// Effectue une rotation droite pour rééquilibrer l'arbre
AvlUsine* rotation_droite_usine(AvlUsine* y);

// Effectue une rotation gauche pour rééquilibrer l'arbre
AvlUsine* rotation_gauche_usine(AvlUsine* x);

// Insertion d'une usine avec rééquilibrage AVL
AvlUsine* inserer_usine(AvlUsine* racine, Usine* u);

// Libération complète de l'arbre AVL
void liberer_avl_usine(AvlUsine* racine);

#endif