#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

// 1. On définit max_int ici pour qu'il soit disponible
static int max_int(int a, int b) {
    return (a > b) ? a : b;
}

// 2. Fonctions de hauteur et équilibre
int hauteur_avl(AVLNode* n) {
    return (n == NULL) ? 0 : n->hauteur;
}

int facteur_equilibre_avl(AVLNode* n) {
    return (n == NULL) ? 0 : hauteur_avl(n->gauche) - hauteur_avl(n->droite);
}

// 3. Création (COPIE de la chaîne obligatoire)
AVLNode* creer_noeud_avl(const char* id, void* ptr_noeud) {
    AVLNode* n = malloc(sizeof(AVLNode));
    if (!n) return NULL;

    // COPIE de la chaîne (on n'utilise pas =)
    // Si ta struct a un "char id[100]", on utilise strcpy
    // Si ta struct a un "char* id", on utilise strdup
    strncpy(n->id, id, sizeof(n->id) - 1);
    n->id[sizeof(n->id) - 1] = '\0'; 

    n->ptr_noeud = ptr_noeud;
    n->gauche = n->droite = NULL;
    n->hauteur = 1;
    return n;
}

// 4. Rotations (Noms propres au réseau)
AVLNode* rotation_droite_avl(AVLNode* y) {
    AVLNode* x = y->gauche;
    AVLNode* T2 = x->droite;
    x->droite = y;
    y->gauche = T2;
    y->hauteur = 1 + max_int(hauteur_avl(y->gauche), hauteur_avl(y->droite));
    x->hauteur = 1 + max_int(hauteur_avl(x->gauche), hauteur_avl(x->droite));
    return x;
}

AVLNode* rotation_gauche_avl(AVLNode* x) {
    AVLNode* y = x->droite;
    AVLNode* T2 = y->gauche;
    y->gauche = x;
    x->droite = T2;
    x->hauteur = 1 + max_int(hauteur_avl(x->gauche), hauteur_avl(x->droite));
    y->hauteur = 1 + max_int(hauteur_avl(y->gauche), hauteur_avl(y->droite));
    return y;
}

// 5. Insertion (Utilisation de STRCMP pour les chaînes)
AVLNode* inserer_noeud_avl(AVLNode* racine, const char* id, void* ptr_noeud) {
    if (racine == NULL) return creer_noeud_avl(id, ptr_noeud);

    // COMPARAISON DE CHAINES
    int cmp = strcmp(id, racine->id);

    if (cmp < 0)
        racine->gauche = inserer_noeud_avl(racine->gauche, id, ptr_noeud);
    else if (cmp > 0)
        racine->droite = inserer_noeud_avl(racine->droite, id, ptr_noeud);
    else
        return racine;

    racine->hauteur = 1 + max_int(hauteur_avl(racine->gauche), hauteur_avl(racine->droite));
    int eq = facteur_equilibre_avl(racine);

    // ÉQUILIBRAGE avec strcmp
    if (eq > 1 && strcmp(id, racine->gauche->id) < 0) return rotation_droite_avl(racine);
    if (eq < -1 && strcmp(id, racine->droite->id) > 0) return rotation_gauche_avl(racine);
    if (eq > 1 && strcmp(id, racine->gauche->id) > 0) {
        racine->gauche = rotation_gauche_avl(racine->gauche);
        return rotation_droite_avl(racine);
    }
    if (eq < -1 && strcmp(id, racine->droite->id) < 0) {
        racine->droite = rotation_droite_avl(racine->droite);
        return rotation_gauche_avl(racine);
    }
    return racine;
}

// 6. Libération
void liberer_avl(AVLNode* racine) {
    if (racine == NULL) return;
    liberer_avl(racine->gauche);
    liberer_avl(racine->droite);
    free(racine);
}
