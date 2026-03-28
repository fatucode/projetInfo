#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

// Retourne la hauteur d’un nœud AVL
// Si le nœud est NULL, la hauteur est 0
int obtenir_hauteur(NoeudAVL* n) {
    return (n == NULL) ? 0 : n->hauteur;
}

// Retourne la valeur maximale entre deux entiers
int max_val(int a, int b) {
    return (a > b) ? a : b;
}

// Rotation simple à droite (cas Gauche-Gauche)
// Permet de rééquilibrer l’arbre AVL
NoeudAVL* rotation_droite(NoeudAVL* y) {
    NoeudAVL* x = y->gauche;
    NoeudAVL* T2 = x->droite;

    // Rotation
    x->droite = y;
    y->gauche = T2;

    // Mise à jour des hauteurs
    y->hauteur = max_val(obtenir_hauteur(y->gauche), obtenir_hauteur(y->droite)) + 1;
    x->hauteur = max_val(obtenir_hauteur(x->gauche), obtenir_hauteur(x->droite)) + 1;

    // Nouvelle racine du sous-arbre
    return x;
}

// Rotation simple à gauche (cas Droite-Droite)
// Permet de rééquilibrer l’arbre AVL
NoeudAVL* rotation_gauche(NoeudAVL* x) {
    NoeudAVL* y = x->droite;
    NoeudAVL* T2 = y->gauche;

    // Rotation
    y->gauche = x;
    x->droite = T2;

    // Mise à jour des hauteurs
    x->hauteur = max_val(obtenir_hauteur(x->gauche), obtenir_hauteur(x->droite)) + 1;
    y->hauteur = max_val(obtenir_hauteur(y->gauche), obtenir_hauteur(y->droite)) + 1;

    // Nouvelle racine du sous-arbre
    return y;
}

// Calcule le facteur d’équilibre d’un nœud AVL
// équilibre = hauteur(gauche) - hauteur(droite)
int obtenir_equilibre(NoeudAVL* n) {
    if (n == NULL) return 0;
    return obtenir_hauteur(n->gauche) - obtenir_hauteur(n->droite);
}

// Recherche d’un nœud dans l’AVL à partir de son identifiant
NoeudAVL* avl_recherche(NoeudAVL* racine, const char* id) {
    if (!racine) return NULL;

    int cmp = strcmp(id, racine->id);

    // Identifiant trouvé
    if (cmp == 0) return racine;
    // Recherche dans le sous-arbre gauche
    else if (cmp < 0) return avl_recherche(racine->gauche, id);
    // Recherche dans le sous-arbre droit
    else return avl_recherche(racine->droite, id);
}

// Insertion d’un nœud dans l’arbre AVL
// L’arbre est automatiquement rééquilibré après insertion
NoeudAVL* avl_insertion(NoeudAVL* racine, const char* id, Noeud* ptr_noeud) {
    // Cas de base : création d’un nouveau nœud
    if (racine == NULL) {
        NoeudAVL* node = malloc(sizeof(NoeudAVL));
        strcpy(node->id, id);
        node->ptr_noeud = ptr_noeud;
        node->gauche = node->droite = NULL;
        node->hauteur = 1;
        return node;
    }

    // Insertion classique dans un arbre binaire de recherche
    int cmp = strcmp(id, racine->id);
    if (cmp < 0)
        racine->gauche = avl_insertion(racine->gauche, id, ptr_noeud);
    else if (cmp > 0)
        racine->droite = avl_insertion(racine->droite, id, ptr_noeud);
    else
        // Les doublons ne sont pas autorisés
        return racine;

    // Mise à jour de la hauteur du nœud courant
    racine->hauteur = 1 + max_val(
        obtenir_hauteur(racine->gauche),
        obtenir_hauteur(racine->droite)
    );

    // Calcul du facteur d’équilibre
    int balance = obtenir_equilibre(racine);

    // Cas Gauche-Gauche
    if (balance > 1 && strcmp(id, racine->gauche->id) < 0)
        return rotation_droite(racine);

    // Cas Droite-Droite
    if (balance < -1 && strcmp(id, racine->droite->id) > 0)
        return rotation_gauche(racine);

    // Cas Gauche-Droite
    if (balance > 1 && strcmp(id, racine->gauche->id) > 0) {
        racine->gauche = rotation_gauche(racine->gauche);
        return rotation_droite(racine);
    }

    // Cas Droite-Gauche
    if (balance < -1 && strcmp(id, racine->droite->id) < 0) {
        racine->droite = rotation_droite(racine->droite);
        return rotation_gauche(racine);
    }

    // Nœud équilibré
    return racine;
}

// Libération complète de la mémoire de l’arbre AVL
void liberer_avl(NoeudAVL* racine) {
    if (!racine) return;
    liberer_avl(racine->gauche);
    liberer_avl(racine->droite);
    free(racine);
}
