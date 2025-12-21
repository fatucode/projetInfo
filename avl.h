#ifndef AVL_H
#define AVL_H

#include "reseaau.h" // Inclusion nécessaire pour que le type Noeud soit reconnu

// Structure d’un nœud de l’arbre AVL
typedef struct NoeudAVL   {
    char id[100];              // Identifiant unique du nœud (clé de l’AVL)
    Noeud* ptr_noeud;          // Pointeur vers le nœud du réseau associé
    struct NoeudAVL *gauche;   // Fils gauche dans l’arbre AVL
    struct NoeudAVL *droite;   // Fils droit dans l’arbre AVL
    int hauteur;               // Hauteur du nœud (utilisée pour l’équilibrage AVL)
} NoeudAVL;

// -------- Prototypes des fonctions AVL --------

// Retourne la hauteur d’un nœud AVL
int obtenir_hauteur(NoeudAVL* n);

// Retourne le maximum entre deux entiers
int max_val(int a, int b);

// Effectue une rotation droite pour rééquilibrer l’arbre
NoeudAVL* rotation_droite(NoeudAVL* y);

// Effectue une rotation gauche pour rééquilibrer l’arbre
NoeudAVL* rotation_gauche(NoeudAVL* x);

// Calcule le facteur d’équilibre d’un nœud AVL
int obtenir_equilibre(NoeudAVL* n);

// Recherche un nœud dans l’AVL à partir de son identifiant
NoeudAVL* avl_recherche(NoeudAVL* racine, const char* id);

// Insère un nouveau nœud dans l’AVL en maintenant l’équilibrage
NoeudAVL* avl_insertion(NoeudAVL* racine, const char* id, Noeud* ptr_noeud);

// Libère récursivement la mémoire de l’arbre AVL
void liberer_avl(NoeudAVL* racine);

#endif
