#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reseaau.h"
#include "avl.h"
#include "csv_reader.h"

/* --------------------------------------------------------
   Création d’un nœud
-------------------------------------------------------- */
Noeud* creer_noeud(const char* id, TypeNoeud type) {
    Noeud* noeud = malloc(sizeof(Noeud));
    if (!noeud) return NULL;

    strncpy(noeud->id, id, MAX_ID);
    noeud->id[MAX_ID - 1] = '\0';

    noeud->type = type;
    noeud->volume = 0.0;
    noeud->fuite = 0.0;
    noeud->enfants = NULL;

    return noeud;
}

/* --------------------------------------------------------
   Ajouter un enfant à un parent
-------------------------------------------------------- */
void ajouter_enfant(Noeud* parent, Noeud* enfant) {
    if (!parent || !enfant) return;

    ChildNode* lien = malloc(sizeof(ChildNode));
    if (!lien) return;

    lien->enfant = enfant;
    lien->suivant = parent->enfants;
    parent->enfants = lien;
}

/* --------------------------------------------------------
   Détection du type de ligne CSV
-------------------------------------------------------- */
TypeLigne detecter_type_ligne(
    const char* c1,
    const char* c2,
    const char* c3,
    const char* c4,
    const char* c5
) {
    if (!c1 || !c2 || !c3 || !c4 || !c5)
        return INCONNU;

    // SOURCE → USINE
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") != 0 &&
        strcmp(c5, "-") != 0)
        return SOURCE_USINE;

    // USINE seule
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") == 0 &&
        strcmp(c4, "-") != 0)
        return USINE_NOEUD;

    // USINE → STOCKAGE
    if (strcmp(c1, "-") == 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") == 0 &&
        strcmp(c5, "-") != 0)
        return USINE_STOCKAGE;

    // TRONÇON DE DISTRIBUTION
    if (strcmp(c1, "-") != 0 &&
        strcmp(c3, "-") != 0 &&
        strcmp(c4, "-") == 0 &&
        strcmp(c5, "-") != 0)
        return TRONCON_DISTRIBUTION;

    return INCONNU;
}

/* --------------------------------------------------------
   Traitement d’une ligne CSV
-------------------------------------------------------- */
void traiter_ligne_csv(const char* c1,
                       const char* c2,
                       const char* c3,
                       const char* c4,
                       const char* c5,
                       void* user_data)
{
    AVLNode** racine = (AVLNode**) user_data;

    TypeLigne type_ligne = detecter_type_ligne(c1, c2, c3, c4, c5);

    const char* amont  = c2;
    const char* aval   = c3;
    const char* volume = c4;
    const char* fuite  = c5;

    if (!amont || !aval || strcmp(amont, "-") == 0 || strcmp(aval, "-") == 0)
        return;

    TypeNoeud type_amont = NOEUD_INCONNU;
    TypeNoeud type_aval  = NOEUD_INCONNU;

    switch (type_ligne) {
        case SOURCE_USINE:
            type_amont = NOEUD_SOURCE;
            type_aval  = NOEUD_USINE;
            break;

        case USINE_NOEUD:
            type_amont = NOEUD_USINE;
            type_aval  = NOEUD_USINE;
            break;

        case USINE_STOCKAGE:
            type_amont = NOEUD_USINE;
            type_aval  = NOEUD_STOCKAGE;
            break;

        case TRONCON_DISTRIBUTION:
            type_amont = NOEUD_RACCORDEMENT;
            type_aval  = NOEUD_USAGER;
            break;

        default:
            return;
    }

    // Recherche ou création du nœud amont
    AVLNode* noeud_amont = avl_recherche(*racine, amont);
    if (!noeud_amont) {
        Noeud* na = creer_noeud(amont, type_amont);
        *racine = avl_insertion(*racine, amont, na);
        noeud_amont = avl_recherche(*racine, amont);
    }

    // Recherche ou création du nœud aval
    AVLNode* noeud_aval = avl_recherche(*racine, aval);
    if (!noeud_aval) {
        Noeud* nb = creer_noeud(aval, type_aval);
        *racine = avl_insertion(*racine, aval, nb);
        noeud_aval = avl_recherche(*racine, aval);
    }

    // Lien parent → enfant
    ajouter_enfant(noeud_amont->ptr_noeud, noeud_aval->ptr_noeud);

    // Volume (production usine)
    if ((type_ligne == SOURCE_USINE || type_ligne == USINE_NOEUD) &&
        volume && strcmp(volume, "-") != 0)
    {
        noeud_aval->ptr_noeud->volume = atof(volume);
    }

    // Fuite
    if (fuite && strcmp(fuite, "-") != 0)
        noeud_aval->ptr_noeud->fuite = atof(fuite);
}

/* --------------------------------------------------------
   Chargement du réseau
-------------------------------------------------------- */
AVLNode* charger_reseau(const char* filename)
{
    AVLNode* racine = NULL;

    if (csv_process_file(filename, traiter_ligne_csv, &racine) != 0) {
        fprintf(stderr, "Erreur : lecture du fichier '%s'\n", filename);
        return NULL;
    }

    if (!racine) {
        fprintf(stderr, "Erreur : réseau vide\n");
        return NULL;
    }

    return racine;
}
