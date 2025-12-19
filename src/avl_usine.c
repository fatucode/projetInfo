#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_usine.h"

int hauteur_usine(AvlUsine* n) {
    return (n == NULL) ? 0 : n->hauteur;
}

static int max_int(int a, int b) {
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
