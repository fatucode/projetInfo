#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_usine.h"

int hauteur_usine(AvlUsine* n) {
    return (n == NULL) ? 0 : n->hauteur;
}

 int max_int(int a, int b) {
    return (a > b) ? a : b;
}

AvlUsine* creer_noeud_usine(Usine* u) {
    AvlUsine* n = malloc(sizeof(AvlUsine));
    if (!n) return NULL;
    n->usine = u;
    n->gauche = n->droite = NULL;
    n->hauteur = 1;
    return n;
}

int facteur_equilibre_usine(AvlUsine* n) {
    return (n == NULL) ? 0 : hauteur_usine(n->gauche) - hauteur_usine(n->droite);
}

AvlUsine* rotation_droite_usine(AvlUsine* y) {
    AvlUsine* x = y->gauche;
    AvlUsine* T2 = x->droite;
    x->droite = y;
    y->gauche = T2;
    y->hauteur = 1 + max_int(hauteur_usine(y->gauche), hauteur_usine(y->droite));
    x->hauteur = 1 + max_int(hauteur_usine(x->gauche), hauteur_usine(x->droite));
    return x;
}

AvlUsine* rotation_gauche_usine(AvlUsine* x) {
    AvlUsine* y = x->droite;
    AvlUsine* T2 = y->gauche;
    y->gauche = x;
    x->droite = T2;
    x->hauteur = 1 + max_int(hauteur_usine(x->gauche), hauteur_usine(x->droite));
    y->hauteur = 1 + max_int(hauteur_usine(y->gauche), hauteur_usine(y->droite));
    return y;
}

AvlUsine* inserer_usine(AvlUsine* racine, Usine* u) {
    if (racine == NULL) return creer_noeud_usine(u);
    int cmp = strcmp(u->id, racine->usine->id);
    if (cmp < 0) racine->gauche = inserer_usine(racine->gauche, u);
    else if (cmp > 0) racine->droite = inserer_usine(racine->droite, u);
    else return racine;

    racine->hauteur = 1 + max_int(hauteur_usine(racine->gauche), hauteur_usine(racine->droite));
    int eq = facteur_equilibre_usine(racine);

    if (eq > 1 && strcmp(u->id, racine->gauche->usine->id) < 0) return rotation_droite_usine(racine);
    if (eq < -1 && strcmp(u->id, racine->droite->usine->id) > 0) return rotation_gauche_usine(racine);
    if (eq > 1 && strcmp(u->id, racine->gauche->usine->id) > 0) {
        racine->gauche = rotation_gauche_usine(racine->gauche);
        return rotation_droite_usine(racine);
    }
    if (eq < -1 && strcmp(u->id, racine->droite->usine->id) < 0) {
        racine->droite = rotation_droite_usine(racine->droite);
        return rotation_gauche_usine(racine);
    }
    return racine;
}

void liberer_avl_usine(AvlUsine* racine) {
    if (racine == NULL) return;
    liberer_avl_usine(racine->gauche);
    liberer_avl_usine(racine->droite);
    if(racine->usine) free(racine->usine);
    free(racine);
}
// FONCTIONS À RAJOUTER DANS AVL_USINE.C 

// Cette fonction est appelée par le main.c (traiter_ligne)
Usine* chercher_usine(AvlUsine* racine, const char* id) {
    if (racine == NULL) {
        return NULL;
    }
    int cmp = strcmp(id, racine->usine->id);
    if (cmp == 0) {
        return racine->usine;
    } else if (cmp < 0) {
        return chercher_usine(racine->gauche, id);
    } else {
        return chercher_usine(racine->droite, id);
    }
}

// Cette fonction est appelée par histogramme.c
void parcours_inverse(AvlUsine* racine, FILE* out, int mode) {
    if (racine == NULL) return;

    // Droite, puis Racine, puis Gauche pour l'ordre décroissant
    parcours_inverse(racine->droite, out, mode);

    Usine* u = racine->usine;
    if (mode == 0) fprintf(out, "%s;%.2f\n", u->id, u->max);
    else if (mode == 1) fprintf(out, "%s;%.2f\n", u->id, u->src);
    else if (mode == 2) fprintf(out, "%s;%.2f\n", u->id, u->real);
    else fprintf(out, "%s;%.2f;%.2f;%.2f\n", u->id, u->max, u->src, u->real);

    parcours_inverse(racine->gauche, out, mode);
}
