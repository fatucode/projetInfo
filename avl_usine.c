#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_usine.h"

// Retourne la hauteur d’un nœud AVL usine
// Si le nœud est NULL, la hauteur est 0
int hauteur_usine(AvlUsine* n) {
    return (n == NULL) ? 0 : n->hauteur;
}

// Retourne le maximum entre deux entiers
int max_int(int a, int b) {
    return (a > b) ? a : b;
}

// Crée un nouveau nœud AVL pour une usine
// Initialise les pointeurs gauche/droite à NULL et la hauteur à 1
AvlUsine* creer_noeud_usine(Usine* u) {
    AvlUsine* n = malloc(sizeof(AvlUsine));
    if (!n) return NULL;
    n->usine = u;
    n->gauche = n->droite = NULL;
    n->hauteur = 1;
    return n;
}

// Calcule le facteur d’équilibre d’un nœud AVL usine
// facteur = hauteur(gauche) - hauteur(droite)
int facteur_equilibre_usine(AvlUsine* n) {
    return (n == NULL) ? 0 : hauteur_usine(n->gauche) - hauteur_usine(n->droite);
}

// Rotation droite pour rééquilibrer l’AVL (cas gauche-gauche)
AvlUsine* rotation_droite_usine(AvlUsine* y) {
    AvlUsine* x = y->gauche;
    AvlUsine* T2 = x->droite;

    // Effectuer la rotation
    x->droite = y;
    y->gauche = T2;

    // Mise à jour des hauteurs
    y->hauteur = 1 + max_int(hauteur_usine(y->gauche), hauteur_usine(y->droite));
    x->hauteur = 1 + max_int(hauteur_usine(x->gauche), hauteur_usine(x->droite));

    return x;
}

// Rotation gauche pour rééquilibrer l’AVL (cas droite-droite)
AvlUsine* rotation_gauche_usine(AvlUsine* x) {
    AvlUsine* y = x->droite;
    AvlUsine* T2 = y->gauche;

    // Effectuer la rotation
    y->gauche = x;
    x->droite = T2;

    // Mise à jour des hauteurs
    x->hauteur = 1 + max_int(hauteur_usine(x->gauche), hauteur_usine(x->droite));
    y->hauteur = 1 + max_int(hauteur_usine(y->gauche), hauteur_usine(y->droite));

    return y;
}

// Insère une usine dans l’AVL en respectant l’ordre lexicographique des identifiants
// Rééquilibre l’arbre si nécessaire
AvlUsine* inserer_usine(AvlUsine* racine, Usine* u) {
    // Cas de base : arbre vide
    if (racine == NULL) return creer_noeud_usine(u);

    // Comparaison des identifiants d’usine
    int cmp = strcmp(u->id, racine->usine->id);

    // Insertion récursive à gauche ou à droite
    if (cmp < 0)
        racine->gauche = inserer_usine(racine->gauche, u);
    else if (cmp > 0)
        racine->droite = inserer_usine(racine->droite, u);
    else
        return racine; // Pas de doublons

    // Mise à jour de la hauteur
    racine->hauteur = 1 + max_int(
        hauteur_usine(racine->gauche),
        hauteur_usine(racine->droite)
    );

    // Calcul du facteur d’équilibre
    int eq = facteur_equilibre_usine(racine);

    // Cas gauche-gauche
    if (eq > 1 && strcmp(u->id, racine->gauche->usine->id) < 0)
        return rotation_droite_usine(racine);

    // Cas droite-droite
    if (eq < -1 && strcmp(u->id, racine->droite->usine->id) > 0)
        return rotation_gauche_usine(racine);

    // Cas gauche-droite
    if (eq > 1 && strcmp(u->id, racine->gauche->usine->id) > 0) {
        racine->gauche = rotation_gauche_usine(racine->gauche);
        return rotation_droite_usine(racine);
    }

    // Cas droite-gauche
    if (eq < -1 && strcmp(u->id, racine->droite->usine->id) < 0) {
        racine->droite = rotation_droite_usine(racine->droite);
        return rotation_gauche_usine(racine);
    }

    return racine;
}

// Libère récursivement toute la mémoire de l’AVL usine
// Libère aussi la structure Usine associée à chaque nœud
void liberer_avl_usine(AvlUsine* racine) {
    if (racine == NULL) return;
    liberer_avl_usine(racine->gauche);
    liberer_avl_usine(racine->droite);
    if (racine->usine) free(racine->usine);
    free(racine);
}

// Recherche une usine par son identifiant dans l’AVL
// Fonction appelée depuis le main.c
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

// Parcours inverse de l’AVL (ordre décroissant)
// Utilisé pour générer des fichiers d’histogrammes
void parcours_inverse(AvlUsine* racine, FILE* out, int mode) {
    if (racine == NULL) return;

    // Parcours droite → racine → gauche (ordre décroissant)
    parcours_inverse(racine->droite, out, mode);

    Usine* u = racine->usine;

    // Écriture selon le mode demandé
    if (mode == 0)
        fprintf(out, "%s;%.2f\n", u->id, u->max);
    else if (mode == 1)
        fprintf(out, "%s;%.2f\n", u->id, u->src);
    else if (mode == 2)
        fprintf(out, "%s;%.2f\n", u->id, u->real);
    else
        fprintf(out, "%s;%.2f;%.2f;%.2f\n", u->id, u->max, u->src, u->real);

    parcours_inverse(racine->gauche, out, mode);
}