#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_usine.h"

// Hauteur d'un noeud
int hauteur(AvlUsine* n) {
    if (n == NULL){
        return 0;
    }
    return n->hauteur;
}

// Maximum entre deux entiers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Créer un noeud AVL
AvlUsine* creer_noeud_usine(Usine* u) {
    AvlUsine* n = malloc(sizeof(AvlUsine));
    if (n ==NULL) {
        return NULL;
    }

    n->usine = u;
    n->gauche = NULL;
    n->droite = NULL;
    n->hauteur = 1;

    return n;
}
// Comparer deux identifiants d'usine
int comparer_id(const char* a, const char* b) {
    return strcmp(a, b);
}
AvlUsine* inserer_usine(AvlUsine* racine, Usine* u) {
    // Insertion BST classique
    if (racine == NULL) {
        return creer_noeud_usine(u);
    }

    int cmp = comparer_id(u->id, racine->usine->id);

    if (cmp < 0)
        racine->gauche = inserer_usine(racine->gauche, u);
    else if (cmp > 0)
        racine->droite = inserer_usine(racine->droite, u);
    else
        return racine; // doublon ignoré

    // Mise à jour hauteur
    racine->hauteur = 1 + max(hauteur(racine->gauche),
                              hauteur(racine->droite));

    // Calcul équilibre
    int eq = facteur_equilibre(racine);

    // Cas gauche-gauche
    if (eq > 1 && comparer_id(u->id, racine->gauche->usine->id) < 0)
        return rotation_droite(racine);

    // Cas droite-droite
    if (eq < -1 && comparer_id(u->id, racine->droite->usine->id) > 0)
        return rotation_gauche(racine);

    // Cas gauche-droite
    if (eq > 1 && comparer_id(u->id, racine->gauche->usine->id) > 0) {
        racine->gauche = rotation_gauche(racine->gauche);
        return rotation_droite(racine);
    }

    // Cas droite-gauche
    if (eq < -1 && comparer_id(u->id, racine->droite->usine->id) < 0) {
        racine->droite = rotation_droite(racine->droite);
        return rotation_gauche(racine);
    }

    return racine;
}
Usine* chercher_usine(AvlUsine* racine, const char* id) {
    if (racine == NULL) {
        return NULL;
    }

    int cmp = comparer_id(id, racine->usine->id);

    if (cmp == 0) {
        return racine->usine;
    }
    else if (cmp < 0) {
        return chercher_usine(racine->gauche, id);
    }
    else {
        return chercher_usine(racine->droite, id);
    }
}
// Facteur d'équilibre d'un noeud
int facteur_equilibre(AvlUsine* n) {
    if (n == NULL){
        return 0;
    }
    return hauteur(n->gauche) - hauteur(n->droite);
}
// Rotation droite
AvlUsine* rotation_droite(AvlUsine* y) {
    AvlUsine* x = y->gauche;
    AvlUsine* T2 = x->droite;

    // Rotation
    x->droite = y;
    y->gauche = T2;

    // Mise à jour des hauteurs
    y->hauteur = 1 + max(hauteur(y->gauche), hauteur(y->droite));
    x->hauteur = 1 + max(hauteur(x->gauche), hauteur(x->droite));

    return x;
}
// Rotation gauche
AvlUsine* rotation_gauche(AvlUsine* x) {
    AvlUsine* y = x->droite;
    AvlUsine* T2 = y->gauche;

    // Rotation
    y->gauche = x;
    x->droite = T2;

    // Mise à jour des hauteurs
    x->hauteur = 1 + max(hauteur(x->gauche), hauteur(x->droite));
    y->hauteur = 1 + max(hauteur(y->gauche), hauteur(y->droite));

    return y;
}
//parcours infixe inverse
void parcours_inverse(AvlUsine* racine, FILE* out, int mode) {
    if (racine == NULL) return;

    parcours_inverse(racine->droite, out, mode);

    Usine* u = racine->usine;

    if (mode == 0) {
        fprintf(out, "%s;%.2f\n", u->id, u->max);
    } else if (mode == 1) {
        fprintf(out, "%s;%.2f\n", u->id, u->src);
    } else if (mode == 2) {
        fprintf(out, "%s;%.2f\n", u->id, u->real);
    } else {
        fprintf(out, "%s;%.2f;%.2f;%.2f\n",
                u->id, u->max, u->src, u->real);
    }

    parcours_inverse(racine->gauche, out, mode);
}

void liberer_avl_usine(AvlUsine* racine) {
    if (racine == NULL){
        return;
    }

    liberer_avl_usine(racine->gauche);
    liberer_avl_usine(racine->droite);

    free(racine->usine);
    free(racine);
}

