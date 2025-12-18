#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reseaau.h"
#include "avl.h"

// --------------------------------------------------------
// Création d’un nœud
// --------------------------------------------------------
Noeud* creer_noeud(const char* id, TypeNoeud type) {
    Noeud* n = malloc(sizeof(Noeud));
    if (!n) return NULL;

    strncpy(n->id, id, MAX_ID);
    n->id[MAX_ID - 1] = '\0';

    n->type = type;
    n->volume = 0.0;
    n->fuite = 0.0;
    n->enfants = NULL;

    return n;
}

// --------------------------------------------------------
// Ajouter un enfant à un parent
// --------------------------------------------------------
void ajouter_enfant(Noeud* parent, Noeud* enfant) {
    if (!parent || !enfant) return;

    ChildNode* c = malloc(sizeof(ChildNode));
    if(c==NULL){
        return;
    }
    c->enfant = enfant;
    c->suivant = parent->enfants;
    parent->enfants = c;
}

// --------------------------------------------------------
// Détecter le type d’un nœud selon son contenu (nom)
// --------------------------------------------------------
TypeNoeud detecter_type(const char* id, int role) {

    if (!id || strcmp(id, "-") == 0)
        return -1;

    // AMONT
    if (role == 1) {
        if (strstr(id, "Spring") || strstr(id, "Well"))
            return SOURCE;

        if (strstr(id, "Facility complex"))
            return USINE;

        if (strstr(id, "Storage"))
            return STOCKAGE;

        if (strstr(id, "Junction"))
            return JONCTION;

        if (strstr(id, "Service"))
            return RACCORDEMENT;
    }

    // AVAL
    if (role == 2) {
        if (strstr(id, "Facility complex"))
            return USINE;

        if (strstr(id, "Storage"))
            return STOCKAGE;

        if (strstr(id, "Junction"))
            return JONCTION;

        if (strstr(id, "Service"))
            return RACCORDEMENT;

        if (strstr(id, "Cust"))
            return USAGER;
    }

    return JONCTION;
}

// --------------------------------------------------------
// Construction du réseau complet depuis le CSV
// --------------------------------------------------------
AVLNode* charger_reseau(const char* filename) {

    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Erreur : impossible d'ouvrir %s\n", filename);
        return NULL;
    }

    AVLNode* avl_root = NULL;
    char ligne[256];

    while (fgets(ligne, sizeof(ligne), f)) {
    char col1[50], col2[50], col3[50], col4[50], col5[50];
    char* volume = NULL;
    char* fuite = NULL;

    // Lecture des 5 colonnes
    int n = sscanf(ligne, "%49[^;];%49[^;];%49[^;];%49[^;];%49[^\n]",
                   col1, col2, col3, col4, col5);

    if (n < 2) continue; // au moins amont et aval

    char* amont = col2;
    char* aval  = col3;
    if (n >= 4) {
         volume = col4;
}
    if (n == 5) {
         fuite = col5;
}

    if (!amont || !aval) continue;

    // Détecter types
    TypeNoeud type_amont = detecter_type(amont, 1);
    TypeNoeud type_aval  = detecter_type(aval, 2);

    // Chercher/amont dans AVL
    AVLNode* node_amont = avl_search(avl_root, amont);
    if (!node_amont) {
        Noeud* n = creer_noeud(amont, type_amont);
        avl_root = avl_insert(avl_root, amont, n);
        node_amont = avl_search(avl_root, amont);
    }

    // Chercher/aval dans AVL
    AVLNode* node_aval = avl_search(avl_root, aval);
    if (!node_aval) {
        Noeud* n = creer_noeud(aval, type_aval);
        avl_root = avl_insert(avl_root, aval, n);
        node_aval = avl_search(avl_root, aval);
    }

    // Ajouter enfant
    ajouter_enfant(node_amont->ptr_noeud, node_aval->ptr_noeud);

    // Volume / fuite
    if (volume && strcmp(volume, "-") != 0)
        node_aval->ptr_noeud->volume = atof(volume);
    if (fuite && strcmp(fuite, "-") != 0)
        node_aval->ptr_noeud->fuite = atof(fuite);
}

    fclose(f);
    return avl_root;
}
